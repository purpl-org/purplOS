/**
 * File: behaviorExploring.cpp
 *
 * Author: ross
 * Created: 2018-03-25
 *
 * Description: exploration behavior
 *
 * Copyright: Anki, Inc. 2018
 *
 **/


#include "engine/aiComponent/behaviorComponent/behaviors/exploring/behaviorExploring.h"

#include "coretech/common/engine/jsonTools.h"
#include "coretech/common/engine/math/polygon.h"
#include "coretech/common/engine/utils/timer.h"
#include "engine/actions/animActions.h"
#include "engine/actions/basicActions.h"
#include "engine/actions/driveToActions.h"
#include "engine/actions/visuallyVerifyActions.h"
#include "engine/aiComponent/behaviorComponent/behaviorContainer.h"
#include "engine/aiComponent/behaviorComponent/behaviorExternalInterface/beiRobotInfo.h"
#include "engine/aiComponent/behaviorComponent/behaviors/exploring/behaviorExploringExamineObstacle.h"
#include "engine/blockWorld/blockWorld.h"
#include "engine/blockWorld/blockWorldFilter.h"
#include "engine/components/pathComponent.h"
#include "engine/components/sensors/proxSensorComponent.h"
#include "engine/navMap/mapComponent.h"
#include "engine/navMap/memoryMap/memoryMapTypes.h"
#include "engine/navMap/memoryMap/data/memoryMapData_ProxObstacle.h"
#include "engine/utils/robotPointSamplerHelper.h"
#include "util/console/consoleInterface.h"
#include "util/logging/DAS.h"
#include "util/random/randomGenerator.h"
#include "util/random/randomIndexSampler.h"
#include "util/random/rejectionSamplerHelper.h"

#define SET_STATE(s) {                          \
  _dVars.state = State::s;                      \
  SetDebugStateName(#s);                        \
  }

#define LOG_CHANNEL "Behaviors"

namespace Anki {
namespace Vector {
  
namespace {
  const char* const kMinSearchRadiusKey = "minSearchRadius_m";
  const char* const kMaxSearchRadiusKey = "maxSearchRadius_m";
  const char* const kMaxChargerDistanceKey = "maxChargerDistance_m";
  const char* const kPAcceptKnownAreasKey = "pAcceptKnownAreas";
  const char* const kAllowNoChargerKey = "allowNoCharger";
  const char* const kMotionProfileKey = "motionProfile";
  
  // todo: params for this:
  const float kMinScanAngle = DEG_TO_RAD( 45.0f );
  const float kMaxScanAngle = DEG_TO_RAD( 140.0f );
  constexpr float kMinCliffPenaltyDist_mm = 100.0f;
  constexpr float kMaxCliffPenaltyDist_mm = 600.0f;
  const float kProxPoseOffset_mm = 120.0f;
  const float kMaxDistToProxPose_mm = 750.0f;
  const float kMinDistToProxPose_mm = 100.0f;
  const float kMaxCubeFromChargerDist_mm = 2000.0f;
  const float kProbReferenceHuman = 1.0f;

  CONSOLE_VAR_RANGED( float, kProbReferenceOnResume, "BehaviorExploring", 1.0f, 0.0f, 1.0f);
  CONSOLE_VAR_RANGED( float, kResumeReferenceCooldown_s, "BehaviorExploring", 20.0f, 0.0f, 60.0f);

  // if no face is known (meaning we can't run the referencing behavior) then run a short face search (at
  // most) this often (instead of driving to a new pose)
  const float kPeriodToCheckForFaces_s = 30.0f;
  
  // If we have driven this far since the last time we looked at the charger, then we should turn and look at the
  // charger before driving again. This helps keep the nav map accurate.
  const float kReferenceChargerDistanceThreshold_mm = 200.f;

  static_assert( kMinCliffPenaltyDist_mm < kMaxCliffPenaltyDist_mm, "Max must be > min" );
  
  // if driving to a goal this far away, it's ok if the robot stops to examine a nearby obstacle
  const float kMinGoalDistForPitStop_mm = 300.0f;
  const float kMinPathCompleteForPitStop = 0.3f; // >= this % of the distance followed
  const float kMaxPathCompleteForPitStop = 0.7f; // <= this % of the distance followed
  
  const float kChargerRadius_mm = 68.89f; // radius of circle that circumscribes the charger
  
  const char* const kDebugName = "BehaviorExploring";
  
  // number of sample positions for new poses. multiple goals are provided so that the planner can choose
  // one, which will probably cause the robot to tend in its current direction when possible
  const unsigned int kNumPositionsForSearch = 2;
  // It's not terribly expensive (todo: check this) to search for multiple pose angles on a given point,
  // so increase this if needed
  const unsigned int kNumAnglesAtPose = 5;
  // number of samples before we give up trying to find a non-colliding pose. Note: the behavior will
  // end if no pose was found
  const unsigned int kNumSampleSteps = 50;
  
  // number of poses to select that are offset from a prox obstacle by some nearby amount
  const unsigned int kNumProxPoses = 1;
  // distance prox obstacles should be from one another to be included in the sample list
  // (set to 0 for an efficiency boost!)
  const float kMinProxObstacleSeparation_mm = 100.0f;
  
  CONSOLE_VAR( bool, kMoveLiftAboveProx, "BehaviorExploring", false);
  
  constexpr MemoryMapTypes::FullContentArray kTypesToBlockSampling =
  {
    {MemoryMapTypes::EContentType::Unknown               , false},
    {MemoryMapTypes::EContentType::ClearOfObstacle       , false},
    {MemoryMapTypes::EContentType::ClearOfCliff          , false},
    {MemoryMapTypes::EContentType::ObstacleObservable    , true },
    {MemoryMapTypes::EContentType::ObstacleProx          , true },
    {MemoryMapTypes::EContentType::ObstacleUnrecognized  , true },
    {MemoryMapTypes::EContentType::Cliff                 , true },
    {MemoryMapTypes::EContentType::InterestingEdge       , true },
    {MemoryMapTypes::EContentType::NotInterestingEdge    , true }
  };
  
  constexpr MemoryMapTypes::FullContentArray kTypesThatAreKnown =
  {
    {MemoryMapTypes::EContentType::Unknown               , false},
    {MemoryMapTypes::EContentType::ClearOfObstacle       , true},
    {MemoryMapTypes::EContentType::ClearOfCliff          , true},
    {MemoryMapTypes::EContentType::ObstacleObservable    , true},
    {MemoryMapTypes::EContentType::ObstacleProx          , true},
    {MemoryMapTypes::EContentType::ObstacleUnrecognized  , true},
    {MemoryMapTypes::EContentType::Cliff                 , true},
    {MemoryMapTypes::EContentType::InterestingEdge       , true},
    {MemoryMapTypes::EContentType::NotInterestingEdge    , true}
  };
  
  using MemoryMapDataConstList = MemoryMapTypes::MemoryMapDataConstList;
  using MemoryMapDataConstPtr = MemoryMapTypes::MemoryMapDataConstPtr;
  
}
  

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BehaviorExploring::DynamicVariables::DynamicVariables()
{
  state = State::Invalid;
  sampledPoses.clear();
  sampledPoses.reserve( kNumProxPoses + kNumPositionsForSearch * kNumAnglesAtPose );
  posesHaveBeenPruned = false;
  distToGoal_mm = -1.0f;
  
  numDriveAttempts = 0;
  hasTakenPitStop = false;
  endReason = "";
  
  devWarnIfNotInterruptedByTick = std::numeric_limits<size_t>::max();
  
  lastSearchForFaceTime_s = -1.f;
  
  timeDeactivated_s = -1.0f;

  gentleInterruptionOKUntilTick = 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BehaviorExploring::BehaviorExploring(const Json::Value& config)
 : ICozmoBehavior(config)
{
  _iConfig.minSearchRadius_m = JsonTools::ParseFloat( config, kMinSearchRadiusKey, kDebugName );
  _iConfig.maxSearchRadius_m = JsonTools::ParseFloat( config, kMaxSearchRadiusKey, kDebugName );
  _iConfig.maxChargerDistance_m = JsonTools::ParseFloat( config, kMaxChargerDistanceKey, kDebugName );
  
  _iConfig.pAcceptKnownAreas = JsonTools::ParseFloat( config, kPAcceptKnownAreasKey, kDebugName );
  
  _iConfig.allowNoCharger = config.get( kAllowNoChargerKey, false ).asBool();

  if( !config[kMotionProfileKey].isNull() ) {
    _iConfig.customMotionProfile = std::make_unique<PathMotionProfile>();
    _iConfig.customMotionProfile->SetFromJSON( config[kMotionProfileKey] );
  }
  
  MakeMemberTunable(_iConfig.minSearchRadius_m, kMinSearchRadiusKey, kDebugName);
  MakeMemberTunable(_iConfig.maxSearchRadius_m, kMaxSearchRadiusKey, kDebugName);
  MakeMemberTunable(_iConfig.maxChargerDistance_m, kMaxChargerDistanceKey, kDebugName);
  MakeMemberTunable(_iConfig.pAcceptKnownAreas, kPAcceptKnownAreasKey, kDebugName);
  
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
BehaviorExploring::~BehaviorExploring()
{
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool BehaviorExploring::WantsToBeActivatedBehavior() const
{
  const bool chargerKnown = _iConfig.allowNoCharger || IsChargerPositionKnown(); // this should also probably be an IsObjectKnown BEICondition
  const bool hasDelegate = _iConfig.examineBehavior != nullptr;
  
  return chargerKnown && hasDelegate;
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::InitBehavior()
{
  const auto& BC = GetBEI().GetBehaviorContainer();
  BC.FindBehaviorByIDAndDowncast( BEHAVIOR_ID(ExploringExamineObstacle),
                                  BEHAVIOR_CLASS(ExploringExamineObstacle),
                                  _iConfig.examineBehavior );
  
  _iConfig.referenceHumanBehavior = BC.FindBehaviorByID( BEHAVIOR_ID(ExploringReferenceHuman) );
  _iConfig.searchForHumanBehavior = BC.FindBehaviorByID( BEHAVIOR_ID(ShortLookAroundForFaceAndCube) );
  
  using namespace RobotPointSamplerHelper;
  _iConfig.openSpacePointEvaluator.reset( new Util::RejectionSamplerHelper<Point2f>() );
  _iConfig.openSpacePolyEvaluator.reset( new Util::RejectionSamplerHelper<Poly2f>() );
  
  // below is a list of factors for rejection sampling. other ideas:
  // how much of a ray covers unknown ground? (totally possible with current accumulators,
  //    but # quads in a line segment unknown, so hard to normalize into a probability)
  // sample only from unknown points... maybe this needs the iNavMap method FindContentIf, and
  //    then sampled from that? depends on how big the return set is and how many quads share a data obj
  // sample only in angles near the current angle? hopefully the planner does this job for us
  
  _iConfig.condHandleNearCharger = _iConfig.openSpacePointEvaluator->AddCondition(
    std::make_shared<RejectIfNotInRange>( 0.0f, M_TO_MM(_iConfig.maxChargerDistance_m) )
  );
  _iConfig.condHandleCliffs = _iConfig.openSpacePointEvaluator->AddCondition(
    std::make_shared<RejectIfWouldCrossCliff>( kMinCliffPenaltyDist_mm )
  );
  _iConfig.condHandleCliffs->SetAcceptanceInterpolant( kMaxCliffPenaltyDist_mm, GetRNG() );
  
  _iConfig.condHandleChargerOutOfView = _iConfig.openSpacePointEvaluator->AddCondition(
    std::make_shared<RejectIfChargerOutOfView>()
  );
  
  _iConfig.condHandleCollisions = _iConfig.openSpacePolyEvaluator->AddCondition(
    std::make_shared<RejectIfCollidesWithMemoryMap>( kTypesToBlockSampling )
  );
  
  _iConfig.condHandleUnknowns = _iConfig.openSpacePolyEvaluator->AddCondition(
    std::make_shared<RejectIfCollidesWithMemoryMap>( kTypesThatAreKnown )
  );
  _iConfig.condHandleUnknowns->SetAcceptanceProbability( _iConfig.pAcceptKnownAreas, GetRNG() );
  
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::GetBehaviorOperationModifiers(BehaviorOperationModifiers& modifiers) const
{
  modifiers.behaviorAlwaysDelegates = false; // take control of CancelSelf()
  // always look for the charger so we know how to get back
  modifiers.visionModesForActivatableScope->insert({ VisionMode::Markers, EVisionUpdateFrequency::Low });
  modifiers.visionModesForActiveScope->insert({
    {VisionMode::Markers, EVisionUpdateFrequency::Low},
    {VisionMode::Faces, EVisionUpdateFrequency::Med}, // so it is able to occasionally look back at faces
    {VisionMode::Pets, EVisionUpdateFrequency::Med} // pets too
  });
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::GetAllDelegates(std::set<IBehavior*>& delegates) const
{
  delegates.insert( _iConfig.examineBehavior.get() );
  delegates.insert( _iConfig.referenceHumanBehavior.get() );
  delegates.insert( _iConfig.searchForHumanBehavior.get() );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::GetBehaviorJsonKeys(std::set<const char*>& expectedKeys) const
{
  const char* list[] = {
    kMinSearchRadiusKey,
    kMaxSearchRadiusKey,
    kMaxChargerDistanceKey,
    kPAcceptKnownAreasKey,
    kAllowNoChargerKey,
    kMotionProfileKey,
  };
  expectedKeys.insert( std::begin(list), std::end(list) );
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::OnBehaviorActivated() 
{
  // if there is a valid end reason, then the last time this behavior ran it stopped on purpose. Otherwise,
  // it's either the first time this behavior has ever run, or we were interrupted last time
  const bool hasEndReason = !_dVars.endReason.empty();
  const bool everRan = _dVars.timeDeactivated_s > 0.0f;

  const float kMaxTimeToCountAsResume_s = 8.0f;
  const float currTime_s = BaseStationTimer::getInstance()->GetCurrentTimeInSeconds();

  const bool isResume = everRan && !hasEndReason && (currTime_s - _dVars.timeDeactivated_s <= kMaxTimeToCountAsResume_s);

  // reset dynamic variables
  _dVars = DynamicVariables();
  _dVars.lastSearchForFaceTime_s = BaseStationTimer::getInstance()->GetCurrentTimeInSeconds();
  
  if( _iConfig.customMotionProfile != nullptr ) {
    SmartSetMotionProfile( *_iConfig.customMotionProfile );
  }

  if( isResume ) {
    if( _iConfig.referenceHumanBehavior->WantsToBeActivated() &&
        ( currTime_s - _iConfig.referenceHumanBehavior->GetTimeActivated_s() > kResumeReferenceCooldown_s ) &&
        (GetRNG().RandDbl() < kProbReferenceOnResume) ) {
      LOG_INFO("BehaviorExploring.OnBehaviorActivated.ResumeReference",
               "do resume reference");
      DelegateIfInControl( _iConfig.referenceHumanBehavior.get(), &BehaviorExploring::SampleAndDrive );
      return;
    }
  }

  // pick a bunch of points and have the planner choose one and drive there
  SampleAndDrive();
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::OnBehaviorDeactivated()
{
  if( !_dVars.endReason.empty() ) {
    DASMSG(behavior_exploring_end, "behavior.exploring.end",
           "Exploring ended (not just an interruption) and the reason why");
    DASMSG_SET(s1, _dVars.endReason, "The reason");
    DASMSG_SEND();
  }

  LOG_INFO("BehaviorExploring.Ended", "reason: %s", _dVars.endReason.empty() ? "<NONE>" : _dVars.endReason.c_str() );

  _dVars.timeDeactivated_s = BaseStationTimer::getInstance()->GetCurrentTimeInSeconds();
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::BehaviorUpdate() 
{
  if( !IsActivated() ) {
    return;
  }
  if( _dVars.state == State::Complete ) {
    if( _dVars.endReason.empty() ) {
      _dVars.endReason = "Unknown";
    }
    CancelSelf();
    return;
  }
  if( GetBEI().GetRobotInfo().IsOnChargerPlatform() ) {
    // user must have placed it on the charger
    _dVars.endReason = "OnCharger";
    CancelSelf();
    return;
  }
  size_t currTick = BaseStationTimer::getInstance()->GetTickCount();
  if( currTick > _dVars.devWarnIfNotInterruptedByTick ) {
    PRINT_NAMED_WARNING("BehaviorExploring.BehaviorUpdate.WasNonInterrupted",
                        "Behavior assumed an interruption that didn't occur. This could mean the robot stops driving");
    _dVars.devWarnIfNotInterruptedByTick = std::numeric_limits<size_t>::max();
    if( _dVars.numDriveAttempts <= 2 ) {
      SampleAndDrive();
    } else {
      // numDriveAttempts is reset every activation, so this should only happen if something internal
      // is cancelling the driving
      PRINT_NAMED_WARNING("BehaviorExploring.BehaviorUpdate.InterruptedMultiple",
                          "Could not start a path without interruption after %d attempts",
                          _dVars.numDriveAttempts);
      _dVars.endReason = "MultipleInterruptions";
      CancelSelf();
    }
    return;
  }

  if( _dVars.state == State::SearchForHuman ) {
    // if we're searching, but could reference instead, then do that by going to the "Arrived" state
    // similarly, if the search ended without finding someone, do the same

    if( _iConfig.referenceHumanBehavior->WantsToBeActivated() ) {
      CancelDelegates();
    }

    if( !IsControlDelegated() ) {
      const bool forceReferencing = true;
      TransitionToArrived(forceReferencing);
    }

    return;
  }

  
  // make sure the lift is out of the prox fov
  PrepRobotForProx();
  const bool isChargerPositionKnown = IsChargerPositionKnown();
  if( !isChargerPositionKnown && !_iConfig.allowNoCharger ) {
    _dVars.endReason = "ChargerUnknown";
    CancelSelf();
  }
  
  // adjust cached poses
  if( !_dVars.posesHaveBeenPruned ) {
    AdjustCachedPoses();
  }
  
  // tell our examine delegate that it's ok to stop mid path if an unexplored obstacle is passed. this
  // is allowed when the robot is driving a long path and is about halfway to the goal (as the bird flies). otherwise,
  // it looks a bit silly that a robot plans a path around something it hasn't examined up close yet
  // ("how does it know it's there if it hasn't examined it?")
  // It might make sense to do the memory map checks here and delegate accordingly, instead of depending
  // on this behavior's WantsToBeActivated, but this way all memory map checks (aside from sampling) are
  // in one behavior
  bool cutDistanceByHalf = false;
  if( (_dVars.distToGoal_mm > kMinGoalDistForPitStop_mm) && !_dVars.sampledPoses.empty() ) {
    const float currDist_mm = CalcDistToCachedGoal( 0 );
    const float perc = currDist_mm / _dVars.distToGoal_mm;
    cutDistanceByHalf = (perc >= kMinPathCompleteForPitStop) && (perc <= kMaxPathCompleteForPitStop);
  }
  _iConfig.examineBehavior->SetCanSeeAndTurnToSideObstacles( cutDistanceByHalf && !_dVars.hasTakenPitStop );
  
  if( !_iConfig.examineBehavior->IsActivated()
      && _iConfig.examineBehavior->WantsToBeActivated() )
  {
    if( cutDistanceByHalf ) {
      _dVars.hasTakenPitStop = true;
      // don't clear cached poses. by keeping them if the robot hasnt reached its goal yet, it means
      // it will resume to drive to the original goal pose.
    } else {
      // clear cached poses
      _dVars.sampledPoses.clear();
    }
    // todo: perhaps this should only start if we've driven far enough from the last spot we examined?
    // this would help with noisy prox data
    DelegateNow( _iConfig.examineBehavior.get(), &BehaviorExploring::RegainedControl );
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::SetGentleInterruptionOKForNow()
{
  const size_t currTick = BaseStationTimer::getInstance()->GetTickCount();

  // add one to let it work next tick too (in most cases, it won't be checked until next tick anyway because
  // our parent already ran)
  _dVars.gentleInterruptionOKUntilTick = currTick + 1;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool BehaviorExploring::CanBeGentlyInterruptedNow() const
{
  const size_t currTick = BaseStationTimer::getInstance()->GetTickCount();
  
  const bool canInterrupt = ( currTick <= _dVars.gentleInterruptionOKUntilTick );

  return canInterrupt;
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool BehaviorExploring::IsChargerPositionKnown() const
{
  return (nullptr != GetCharger());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const ObservableObject* BehaviorExploring::GetCharger() const
{
  BlockWorldFilter chargerFilter;
  chargerFilter.AddAllowedType(ObjectType::Charger_Basic);
  
  std::vector<const ObservableObject*> locatedChargers;
  GetBEI().GetBlockWorld().FindLocatedMatchingObjects(chargerFilter, locatedChargers);
  
  if( !locatedChargers.empty() ) {
    return locatedChargers.front();
  } else {
    return nullptr;
  }
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool BehaviorExploring::IsCubeNearCharger() const
{
  bool retClose = false;
  BlockWorldFilter cubeFilter;
  cubeFilter.AddFilterFcn(&BlockWorldFilter::IsLightCubeFilter);
  
  const auto* charger = GetCharger();
  if( charger != nullptr ) {
    const ObservableObject* cube = GetBEI().GetBlockWorld().FindLocatedObjectClosestTo( charger->GetPose(), cubeFilter );
    
    if( cube != nullptr ) {
      Point2f cubePos{ cube->GetPose().GetTranslation() };
      Point2f chargerPos{ charger->GetPose().GetTranslation() };
      const float distSq = (cubePos - chargerPos).LengthSq();
      if( distSq < kMaxCubeFromChargerDist_mm*kMaxCubeFromChargerDist_mm ) {
        retClose = true;
      }
    }
  }
  
  return retClose;
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::SampleAndDrive()
{
  // if we don't have a face (can't run the referencing behavior) then we should occasionally search for
  // faces. Check that here
  if( ! _iConfig.referenceHumanBehavior->WantsToBeActivated() ) {
    const float currTime_s = BaseStationTimer::getInstance()->GetCurrentTimeInSeconds();
    if( currTime_s - _dVars.lastSearchForFaceTime_s >= kPeriodToCheckForFaces_s ) {
      TransitionToHumanSearch();
      return;
    }
  }
  
  const auto* charger = GetCharger();
  if (charger != nullptr) {
    // See if we've traveled far enough away from the last pose at which we looked at the charger
    const auto& robotPose = GetBEI().GetRobotInfo().GetPose();
    f32 distance = 0.f;
    const bool distanceValid = ComputeDistanceBetween(_dVars.lastReferenceChargerPose, robotPose, distance);
    const bool shouldReferenceCharger = !distanceValid || (distance > kReferenceChargerDistanceThreshold_mm);

    if (shouldReferenceCharger) {
      auto* action = new CompoundActionSequential();
      action->AddAction(new TurnTowardsObjectAction(charger->GetID()));
      action->AddAction(new VisuallyVerifyObjectAction(charger->GetID()));
      DelegateIfInControl(action, &BehaviorExploring::SampleAndDrive);
      _dVars.lastReferenceChargerPose = robotPose;
      return;
    }
  }

  // sample locations to visit
  _dVars.sampledPoses = SampleVisitLocations();
  _dVars.posesHaveBeenPruned = false;
  _dVars.hasTakenPitStop = false;
  
  if( _dVars.sampledPoses.empty() ) {
    // flag to CancelSelf, making sure CancelSelf doesn't happen on the same tick as activation
    SET_STATE(Complete);
    _dVars.endReason = "NoSamplePoses";
    return;
  }
  
  TransitionToDriving();
  
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::TransitionToHumanSearch()
{
  SET_STATE(SearchForHuman);

  SetGentleInterruptionOKForNow();

  if( _iConfig.searchForHumanBehavior->WantsToBeActivated() ) {
    // run search behavior, transition out handled in BehaviorUpdate
    DelegateIfInControl(_iConfig.searchForHumanBehavior.get());

    const float currTime_s = BaseStationTimer::getInstance()->GetCurrentTimeInSeconds();
    _dVars.lastSearchForFaceTime_s = currTime_s;
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::RegainedControl()
{
  // We're in between actions, so an interruption would be alright
  SetGentleInterruptionOKForNow();

  if( _dVars.sampledPoses.empty() ) {
    SampleAndDrive();
  } else {
    TransitionToDriving();
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::TransitionToDriving()
{
  SET_STATE(Driving);
  ++_dVars.numDriveAttempts;
  
  
  auto* action = new CompoundActionSequential();
  
  if( kMoveLiftAboveProx ) {
    action->AddAction( new MoveLiftToHeightAction( MoveLiftToHeightAction::Preset::JUST_ABOVE_PROX ) );
  }
  
  action->AddAction( new DriveToPoseAction( _dVars.sampledPoses ) );
  
  DelegateIfInControl( action, [this](ActionResult res) {
    if( res == ActionResult::CANCELLED_WHILE_RUNNING ){
      // something interrupted this action (ReactToCliff/OnBack/etc.). don't delegate again, since
      // hopefully that means something higher up is interrupting. This case is here to prevent
      // the sampling computation if this behavior is just going to get interrupted. Just to be sure,
      _dVars.devWarnIfNotInterruptedByTick = 2 + BaseStationTimer::getInstance()->GetTickCount();
    } else if( res != ActionResult::SUCCESS ) {
      // this can happen if we cleared all but one of the goal poses, then the robot stopped to
      // examine something midway, then when trying to start again, there is no path to the selected
      // goal. try a couple more times (maybe this needs more precise ActionResult types?)
      if( _dVars.numDriveAttempts <= 5 ) {
        if( (res == ActionResult::PATH_PLANNING_FAILED_ABORT) || (res == ActionResult::FAILED_TRAVERSING_PATH) ) {
          // it's possible noise from the prox sensor is causing a legitimate planner failure (timeout), so
          // do a quick point turn to hopefully find an escape before continuing
          const float angle = (GetRNG().RandDbl() > 0.5f) ? M_PI_2_F : -M_PI_2_F;
          const bool isAbsolute = false;
          auto* action = new TurnInPlaceAction{ angle, isAbsolute };
          DelegateIfInControl( action, [this](ActionResult res) {
            SampleAndDrive();
          });
        } else {
          SampleAndDrive();
        }
      } else {
        LOG_INFO("BehaviorExploring.TransitionToDriving.NoPath",
                 "Could not plan a path after %d attempts",
                 _dVars.numDriveAttempts);
        _dVars.endReason = "CouldNotPlan";
        CancelSelf();
      }
    } else {
      // arrived at a point.
      TransitionToArrived();
    }
  });
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::TransitionToArrived(const bool forceReferencing)
{
  SET_STATE(Arrived);
  _dVars.sampledPoses.clear();
  _dVars.distToGoal_mm = -1.0f;
  _dVars.numDriveAttempts = 0;
  
  auto callback = [this]() {
    auto* action = new CompoundActionSequential();
    const float r = GetRNG().RandDbl();
    bool animFirst = (r < 0.5f);
    
    // anim either goes before or after a random turn
    if( animFirst ) {
      action->AddAction( new TriggerLiftSafeAnimationAction( AnimationTrigger::ExploringLookAround ) );
    }
    
    const float angleChange = GetRNG().RandDblInRange( kMinScanAngle, kMaxScanAngle );
    const bool isAbsAngle = false;
    auto* turnAction = new TurnInPlaceAction( angleChange, isAbsAngle );
    turnAction->SetMaxSpeed(M_PI_2);
    action->AddAction( turnAction );
    
    if( !animFirst ) {
      action->AddAction( new TriggerLiftSafeAnimationAction( AnimationTrigger::ExploringLookAround ) );
    }
    
    DelegateNow( action, [this](const ActionResult& res) {
      // this could get canceled if an obstacle is seen
      SampleAndDrive();
    });
  };
  
  if( _iConfig.referenceHumanBehavior->WantsToBeActivated() &&
      ( forceReferencing || (GetRNG().RandDbl() < kProbReferenceHuman) ) ) {
    DelegateIfInControl( _iConfig.referenceHumanBehavior.get(), callback );
  } else {
    callback();
  }
  
  
  
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::vector<Pose3d> BehaviorExploring::SampleVisitLocations() const
{
  std::vector<Pose3d> retPoses;
  retPoses.reserve( kNumProxPoses + kNumPositionsForSearch*kNumAnglesAtPose );
  
  const auto& robotInfo = GetBEI().GetRobotInfo();
  const auto  memoryMap = GetBEI().GetMapComponent().GetCurrentMemoryMap();
  DEV_ASSERT( nullptr != memoryMap, "BehaviorExploring.SampleVisitLocations.NeedMemoryMap" );
  
  const ObservableObject* charger = GetCharger();
  if( charger == nullptr && !_iConfig.allowNoCharger ) {
    return retPoses;
  }
  
  const Pose3d currRobotPose = robotInfo.GetPose();
  
  // all the logic below works just fine if the charger position is centered directly on the robot.
  // we use that if the charger positions is not known, which can happen if this behavior is started with
  // a voice command
  const bool chargerEqualsRobot = ( charger == nullptr && _iConfig.allowNoCharger );
  const auto& chargerPose = chargerEqualsRobot ? currRobotPose : charger->GetPose();
  
  // check how far the robot is from the charger.. if it's far enough, only sample points to get it closer
  Point2f chargerPos{ chargerPose.GetTranslation() };
  Point2f currRobotPos{ currRobotPose.GetTranslation() };
  const float distFromCharger_mm = ComputeDistanceBetween( chargerPos, currRobotPos );
  
  // don't attempt rejection sampling if the area of intersection of the two circles is small
  // todo: this could be more smartly derived from the eq for area of intersection of two circles,
  // and then pick a minimum area. instead, here's a magic number
  const bool tooFarFromCharger = distFromCharger_mm >= 0.75f*M_TO_MM(_iConfig.maxSearchRadius_m + _iConfig.maxChargerDistance_m);
  
  if( (kNumPositionsForSearch > 0) || tooFarFromCharger ) {
    SampleVisitLocationsOpenSpace( memoryMap,
                                   tooFarFromCharger,
                                   chargerEqualsRobot,
                                   chargerPose,
                                   currRobotPos,
                                   retPoses );
  }
  
  // also sample poses pointing at known but unexplored prox obstacle
  if( (kNumProxPoses > 0) && !tooFarFromCharger ) {
    SampleVisitLocationsFacingObstacle( memoryMap, charger, currRobotPos, retPoses );
  }

  return retPoses;
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::SampleVisitLocationsOpenSpace( std::shared_ptr<const INavMap> memoryMap,
                                                       bool tooFarFromCharger,
                                                       bool chargerEqualsRobot,
                                                       const Pose3d& chargerPose,
                                                       const Point2f& robotPos,
                                                       std::vector<Pose3d>& retPoses ) const
{
  const auto& robotInfo = GetBEI().GetRobotInfo();
  
  int numAcceptedPoses = 0;
  
  float r1;
  float r2;
  if( !tooFarFromCharger ) {
    r1 = M_TO_MM(_iConfig.minSearchRadius_m);
    r2 = M_TO_MM(_iConfig.maxSearchRadius_m);
  } else {
    r1 = kChargerRadius_mm;
    r2 = M_TO_MM(_iConfig.maxChargerDistance_m);
  }
  
  // update cliff positions
  _iConfig.condHandleCliffs->SetRobotPosition( robotPos );
  _iConfig.condHandleCliffs->UpdateCliffs( memoryMap );
  
  // update charger position
  Point2f chargerPosition(chargerPose.GetTranslation());
  _iConfig.condHandleNearCharger->SetOtherPosition( chargerPosition );
  
  _iConfig.condHandleChargerOutOfView->SetChargerPose( chargerPose );
  // If the charger pose is actually just the robot pose, then we don't want this check. So set the acceptance
  // probability to 1.
  if (chargerEqualsRobot) {
    _iConfig.condHandleChargerOutOfView->SetAcceptanceProbability(1.f, GetRNG());
  } else {
    _iConfig.condHandleChargerOutOfView->ClearAcceptanceProbability();
  }
  
  // update memory map
  _iConfig.condHandleCollisions->SetMemoryMap( memoryMap );
  _iConfig.condHandleUnknowns->SetMemoryMap( memoryMap );
  
  for( unsigned int cnt=0; cnt<kNumSampleSteps; ++cnt ) {
    
    // sample a point based on either the robot position or charger position
    Point2f sampledPos = tooFarFromCharger ? chargerPosition : robotPos;
    {
      const Point2f pt = RobotPointSamplerHelper::SamplePointInAnnulus( GetRNG(), r1, r2 );
      sampledPos.x() += pt.x();
      sampledPos.y() += pt.y();
    }
    
    const bool acceptedPoint = _iConfig.openSpacePointEvaluator->Evaluate( GetRNG(), sampledPos );
    
    if( !acceptedPoint ) {
      continue;
    }
    
    // choose a random angle about the Z axis to create a full test pose
    const float angle = M_TWO_PI * static_cast<float>( GetRNG().RandDbl() );
    Pose2d testPose( Radians{angle}, sampledPos );
    
    Quad2f quad = robotInfo.GetBoundingQuadXY( Pose3d{testPose} );
    Poly2f footprint;
    footprint.ImportQuad2d(quad);
    
    const bool acceptedFootprint = _iConfig.openSpacePolyEvaluator->Evaluate( GetRNG(), footprint );
    
    if( !acceptedFootprint ) {
      continue;
    }
    
    // if we get here, accept!!
    ++numAcceptedPoses;
    
    // Convert to full 3D pose in robot world origin:
    Pose3d testPose3d{ testPose };
    testPose3d.SetParent(robotInfo.GetWorldOrigin());
    retPoses.push_back( testPose3d );
    
    // if desired, add additional poses with the same position but different orientations. this should
    // be pretty cheap compared to a pose at a different location
    for( size_t idxAddtl=0; idxAddtl<kNumAnglesAtPose; ++idxAddtl ) {
      const float otherAngle = M_TWO_PI * static_cast<float>( GetRNG().RandDbl() );
      testPose3d.SetRotation( Radians{otherAngle}, Z_AXIS_3D() );
      retPoses.push_back( testPose3d );
    }
    
    if( numAcceptedPoses >= kNumPositionsForSearch ) {
      LOG_INFO("BehaviorExploring.SampleVisitLocationsOpenSpace.Completed",
               "Met required sampling of %d points, cnt=%d", numAcceptedPoses, cnt);
      break;
    }
  }
  
  if( numAcceptedPoses < kNumPositionsForSearch ) {
    PRINT_NAMED_WARNING( "BehaviorExploring.SampleVisitLocationOpenSpace.Unfinished",
                         "Sampled only %d accepted positions(%zu poses) out of expected %d with %d attempts",
                         numAcceptedPoses,
                         retPoses.size(),
                         kNumPositionsForSearch,
                         kNumSampleSteps );
  }
}
  
void BehaviorExploring::SampleVisitLocationsFacingObstacle( std::shared_ptr<const INavMap> memoryMap,
                                                            const ObservableObject* charger,
                                                            const Point2f& robotPos,
                                                            std::vector<Pose3d>& retPoses ) const
{
  MemoryMapTypes::MemoryMapDataConstList unexploredProxObstacles;
  MemoryMapTypes::NodePredicate findFunc = [](MemoryMapTypes::MemoryMapDataConstPtr d){
    if( d->type == MemoryMapTypes::EContentType::ObstacleProx ) {
      auto castPtr = MemoryMapData::MemoryMapDataCast<MemoryMapData_ProxObstacle>( d );
      return !castPtr->IsExplored();
    } else {
      return false;
    }
  };
  if( charger != nullptr ) {
    // build a quad around the charger that fits within the charger circle
    Quad2f chargerFootprint = charger->GetBoundingQuadXY();
    const float chargerScaleFactor = M_TO_MM(_iConfig.maxChargerDistance_m) / kChargerRadius_mm;
    chargerFootprint.Scale( chargerScaleFactor );
    Poly2f chargerPoly;
    chargerPoly.ImportQuad2d(chargerFootprint);
    
    memoryMap->FindContentIf(findFunc, unexploredProxObstacles, FastPolygon(chargerPoly));
  } else {
    memoryMap->FindContentIf(findFunc, unexploredProxObstacles);
  }
  
  // returns a pose that is kProxPoseOffset_mm away from ptr and antiparallel to its normal
  auto getOffsetPose = [this](const MemoryMapDataConstPtr& ptr) {
    auto castPtr = MemoryMapData::MemoryMapDataCast<const MemoryMapData_ProxObstacle>( ptr );
    Pose3d pose{ castPtr->GetObservationPose().GetAngle(),
                 Z_AXIS_3D(),
                 { castPtr->GetObservationPose().GetTranslation().x(), castPtr->GetObservationPose().GetTranslation().y(), 0.0f},
                 GetBEI().GetRobotInfo().GetWorldOrigin() };
    pose.TranslateForward(-kProxPoseOffset_mm);
    return pose;
  };
  
  const auto currListBegin = retPoses.begin(); // to mark the position of the beginning before adding more
  // not too close to existing sampled poses, and close to the robot
  auto isPositionedWell = [&currListBegin,&robotPos]( const MemoryMapDataWrapper<const MemoryMapData_ProxObstacle>& data,
                                            const std::vector<Pose3d>& existing ) {
    Point2f dataPoint { data->GetObservationPose().GetTranslation() };
    const float distSq = (dataPoint - robotPos).LengthSq();
    if( (distSq > kMaxDistToProxPose_mm*kMaxDistToProxPose_mm)
        || (distSq < kMinDistToProxPose_mm*kMinDistToProxPose_mm) )
    {
      return false;
    }
    for( auto itOthers = currListBegin; itOthers != existing.end(); ++itOthers ) {
      Point2f otherPoint{ itOthers->GetTranslation() };
      const float distSq = (otherPoint - dataPoint).LengthSq();
      if( distSq < kMinProxObstacleSeparation_mm*kMinProxObstacleSeparation_mm ) {
        return false;
      }
    }
    return true;
  };
  
  
  if( unexploredProxObstacles.size() <= kNumProxPoses ) {
    // copy all poses that arent too close to one another (greedy)
    for( auto it = unexploredProxObstacles.begin(); it != unexploredProxObstacles.end(); ++it ) {
      bool add = true;
      if( it != unexploredProxObstacles.begin() ) {
        auto castPtr = MemoryMapData::MemoryMapDataCast<const MemoryMapData_ProxObstacle>( *it );
        add = isPositionedWell( castPtr, retPoses );
      }
      if( add ) {
        retPoses.push_back( getOffsetPose(*it) );
      }
    }
  } else {
    // randomly sample, but do it sample by sample so that we can reject two samples that are close together.
    // unfortunately the starting container is an unordered_set, which makes sampling less efficient,
    // so first transfer all the ptrs to a random access list
    std::vector<MemoryMapDataConstPtr> obstacleVec( unexploredProxObstacles.begin(), unexploredProxObstacles.end() );
    std::vector<size_t> acceptedList;
    
    int obstacleCount = static_cast<int>( obstacleVec.size() );
    Anki::Util::RandomIndexSampler sampler( obstacleCount,
                                            obstacleCount,
                                            Anki::Util::RandomIndexSampler::ORDER_SHOULD_BE_RANDOM );
    for( int sampleCnt=0; sampleCnt<obstacleCount; ++sampleCnt ) {
      int nextSample = sampler.GetNext( GetRNG() );
      const auto& sampled = obstacleVec[nextSample];
      auto castPtr = MemoryMapData::MemoryMapDataCast<const MemoryMapData_ProxObstacle>( sampled );
      const bool positionedWell = isPositionedWell( castPtr, retPoses );
      if( positionedWell ) {
        // accept!
        retPoses.push_back( getOffsetPose( sampled ) );
        
        if( retPoses.end() - currListBegin > kNumProxPoses ) {
          break;
        }
      }
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::PrepRobotForProx()
{
  const auto& proxSensor = GetBEI().GetComponentWrapper(BEIComponentID::ProxSensor).GetComponent<ProxSensorComponent>();
  const auto& proxData = proxSensor.GetLatestProxData();
  if( !proxData.foundObject ) {
    if( !IsControlDelegated() && proxData.isLiftInFOV ) {
      auto preset = kMoveLiftAboveProx ? MoveLiftToHeightAction::Preset::JUST_ABOVE_PROX : MoveLiftToHeightAction::Preset::LOW_DOCK;
      DelegateIfInControl( new MoveLiftToHeightAction(preset) );
    }
  }
}
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void BehaviorExploring::AdjustCachedPoses()
{
  const auto& pathComponent = GetBEI().GetRobotInfo().GetPathComponent();
  if( pathComponent.HasPathToFollow() ) {
    auto indexPtr = pathComponent.GetLastSelectedIndex();
    if( indexPtr ) {
      int idx = static_cast<int>(*indexPtr);
      if( (idx >= 0) && (idx < _dVars.sampledPoses.size()) ) {
        _dVars.distToGoal_mm = CalcDistToCachedGoal( idx );
        if( _dVars.sampledPoses.size() > 1 ) {
          // remove all but idx
          _dVars.sampledPoses = std::vector<Pose3d>( {_dVars.sampledPoses[idx]} );
        }
        _dVars.posesHaveBeenPruned = true;
      }
    }
  }
}
 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
float BehaviorExploring::CalcDistToCachedGoal( int idx ) const
{
  const auto& robotInfo = GetBEI().GetRobotInfo();
  Point2f currRobotPos{ robotInfo.GetPose().GetTranslation() };
  const auto& selectedPose = _dVars.sampledPoses[idx];
  Point2f goalPos{ selectedPose.GetTranslation() };
  const float dist_mm = ComputeDistanceBetween( goalPos, currRobotPos );
  return dist_mm;
}
  
}
}
