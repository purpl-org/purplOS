// This file is manually generated using ./tools/ai/generateBehaviorCode.py
// To add a behavior, just create the .cpp and .h files in the correct places
// and re-run ./tools/ai/generateBehavior.py

// This factory creates behaviors from behavior JSONs with a specified
// `behavior_class` where the behavior C++ class name and file name both match
// Behavior{behavior_class}.h/cpp

#include "engine/aiComponent/behaviorComponent/behaviorFactory.h"

#include "engine/aiComponent/behaviorComponent/behaviors/behaviorGreetAfterLongTime.h"
#include "engine/aiComponent/behaviorComponent/behaviors/behaviorHighLevelAI.h"
#include "engine/aiComponent/behaviorComponent/behaviors/behaviorLookAroundInPlace.h"
#include "engine/aiComponent/behaviorComponent/behaviors/behaviorResetState.h"
#include "engine/aiComponent/behaviorComponent/behaviors/behaviorSimpleVoiceResponse.h"
#include "engine/aiComponent/behaviorComponent/behaviors/behaviorStayOnChargerUntilCharged.h"
#include "engine/aiComponent/behaviorComponent/behaviors/behaviorWait.h"
#include "engine/aiComponent/behaviorComponent/behaviors/alexa/behaviorAlexa.h"
#include "engine/aiComponent/behaviorComponent/behaviors/alexa/behaviorAlexaSignInOut.h"
#include "engine/aiComponent/behaviorComponent/behaviors/animationWrappers/behaviorAnimGetInLoop.h"
#include "engine/aiComponent/behaviorComponent/behaviors/animationWrappers/behaviorAnimSequence.h"
#include "engine/aiComponent/behaviorComponent/behaviors/animationWrappers/behaviorAnimSequenceWithFace.h"
#include "engine/aiComponent/behaviorComponent/behaviors/animationWrappers/behaviorAnimSequenceWithObject.h"
#include "engine/aiComponent/behaviorComponent/behaviors/animationWrappers/behaviorCountingAnimation.h"
#include "engine/aiComponent/behaviorComponent/behaviors/animationWrappers/behaviorTextToSpeechLoop.h"
#include "engine/aiComponent/behaviorComponent/behaviors/appBehaviors/behaviorEyeColor.h"
#include "engine/aiComponent/behaviorComponent/behaviors/attentionTransfer/behaviorAttentionTransferIfNeeded.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicCubeInteractions/behaviorPickUpCube.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicCubeInteractions/behaviorPutDownBlock.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicCubeInteractions/behaviorPutDownBlockAtPose.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicCubeInteractions/behaviorRollBlock.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorBumpObject.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorClearChargerArea.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorDriveOffCharger.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorFetchCube.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorFindCube.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorFindCubeAndThen.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorFindFaces.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorFindHome.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorGoHome.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorInteractWithFaces.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorLookAtFaceInFront.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorMoveHeadToAngle.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorPlaceCubeByCharger.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorPopAWheelie.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorRequestToGoHome.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorTurn.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorTurnToFace.h"
#include "engine/aiComponent/behaviorComponent/behaviors/basicWorldInteractions/behaviorWiggleOntoChargerContacts.h"
#include "engine/aiComponent/behaviorComponent/behaviors/blackjack/behaviorBlackJack.h"
#include "engine/aiComponent/behaviorComponent/behaviors/character/howOldAreYou/behaviorHowOldAreYou.h"
#include "engine/aiComponent/behaviorComponent/behaviors/coordinators/behaviorCoordinateGlobalInterrupts.h"
#include "engine/aiComponent/behaviorComponent/behaviors/coordinators/behaviorCoordinateInHabitat.h"
#include "engine/aiComponent/behaviorComponent/behaviors/coordinators/behaviorCoordinateWhileHeldInPalm.h"
#include "engine/aiComponent/behaviorComponent/behaviors/coordinators/behaviorCoordinateWhileInAir.h"
#include "engine/aiComponent/behaviorComponent/behaviors/coordinators/behaviorQuietModeCoordinator.h"
#include "engine/aiComponent/behaviorComponent/behaviors/cubeSpinner/behaviorVectorPlaysCubeSpinner.h"
#include "engine/aiComponent/behaviorComponent/behaviors/danceToTheBeat/behaviorDanceToTheBeat.h"
#include "engine/aiComponent/behaviorComponent/behaviors/danceToTheBeat/behaviorDanceToTheBeatCoordinator.h"
#include "engine/aiComponent/behaviorComponent/behaviors/danceToTheBeat/behaviorListenForBeats.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorCubeDrive.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevBatteryLogging.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevCubeSpinnerConsole.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevDesignCubeLights.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevDisplayReadingsOnFace.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevEventSequenceCapture.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevImageCapture.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevSquawkBoxTest.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevTestBlackjackViz.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevTestSpriteBoxRemaps.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevTouchDataCollection.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevTurnInPlaceTest.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDevViewCubeBackpackLights.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDispatchAfterShake.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorDockingTestSimple.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorFactoryCentroidExtractor.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorLiftLoadTest.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorPlannerTest.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorPowerSaveStressTest.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorPowerSaveTest.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorReactToBody.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/behaviorReactToGazeDirection.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTest.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestButton.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestDockWithCharger.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestDriftCheck.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestDriveForwards.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestInitChecks.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestLookAtCharger.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestMotorCalibration.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestPickup.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestPutOnCharger.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestScreenAndBackpack.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestSoundCheck.h"
#include "engine/aiComponent/behaviorComponent/behaviors/devBehaviors/selfTest/behaviorSelfTestTouch.h"
#include "engine/aiComponent/behaviorComponent/behaviors/dispatch/behaviorDispatcherPassThrough.h"
#include "engine/aiComponent/behaviorComponent/behaviors/dispatch/behaviorDispatcherQueue.h"
#include "engine/aiComponent/behaviorComponent/behaviors/dispatch/behaviorDispatcherRandom.h"
#include "engine/aiComponent/behaviorComponent/behaviors/dispatch/behaviorDispatcherRerun.h"
#include "engine/aiComponent/behaviorComponent/behaviors/dispatch/behaviorDispatcherScoring.h"
#include "engine/aiComponent/behaviorComponent/behaviors/dispatch/behaviorDispatcherStrictPriority.h"
#include "engine/aiComponent/behaviorComponent/behaviors/dispatch/behaviorDispatcherStrictPriorityWithCooldown.h"
#include "engine/aiComponent/behaviorComponent/behaviors/exploring/behaviorExploring.h"
#include "engine/aiComponent/behaviorComponent/behaviors/exploring/behaviorExploringExamineObstacle.h"
#include "engine/aiComponent/behaviorComponent/behaviors/eyeColor/behaviorEyeColorVoiceCommand.h"
#include "engine/aiComponent/behaviorComponent/behaviors/freeplay/putDownDispatch/behaviorLookForFaceAndCube.h"
#include "engine/aiComponent/behaviorComponent/behaviors/freeplay/userInteractive/behaviorFistBump.h"
#include "engine/aiComponent/behaviorComponent/behaviors/freeplay/userInteractive/behaviorInspectCube.h"
#include "engine/aiComponent/behaviorComponent/behaviors/freeplay/userInteractive/behaviorKeepaway.h"
#include "engine/aiComponent/behaviorComponent/behaviors/freeplay/userInteractive/behaviorPounceWithProx.h"
#include "engine/aiComponent/behaviorComponent/behaviors/freeplay/userInteractive/behaviorPuzzleMaze.h"
#include "engine/aiComponent/behaviorComponent/behaviors/gateBehaviors/behaviorConnectToCube.h"
#include "engine/aiComponent/behaviorComponent/behaviors/habitat/behaviorConfirmHabitat.h"
#include "engine/aiComponent/behaviorComponent/behaviors/heldInPalm/behaviorInitialHeldInPalmReaction.h"
#include "engine/aiComponent/behaviorComponent/behaviors/heldInPalm/behaviorReactToPalmEdge.h"
#include "engine/aiComponent/behaviorComponent/behaviors/knowledgeGraph/behaviorKnowledgeGraphQuestion.h"
#include "engine/aiComponent/behaviorComponent/behaviors/meetCozmo/behaviorEnrollFace.h"
#include "engine/aiComponent/behaviorComponent/behaviors/meetCozmo/behaviorRespondToRenameFace.h"
#include "engine/aiComponent/behaviorComponent/behaviors/messaging/behaviorLeaveAMessage.h"
#include "engine/aiComponent/behaviorComponent/behaviors/messaging/behaviorPlaybackMessage.h"
#include "engine/aiComponent/behaviorComponent/behaviors/observing/behaviorObservingLookAtFaces.h"
#include "engine/aiComponent/behaviorComponent/behaviors/observing/behaviorObservingWithoutTurn.h"
#include "engine/aiComponent/behaviorComponent/behaviors/onboarding/behaviorOnboardingCoordinator.h"
#include "engine/aiComponent/behaviorComponent/behaviors/onboarding/phases/behaviorOnboardingEmulate1p0WaitForVC.h"
#include "engine/aiComponent/behaviorComponent/behaviors/onboarding/phases/behaviorOnboardingLookAtPhone.h"
#include "engine/aiComponent/behaviorComponent/behaviors/onboarding/phases/behaviorOnboardingLookAtUser.h"
#include "engine/aiComponent/behaviorComponent/behaviors/onboarding/phases/behaviorOnboardingTeachWakeWord.h"
#include "engine/aiComponent/behaviorComponent/behaviors/onboarding/phases/behaviorOnboardingWakeUp.h"
#include "engine/aiComponent/behaviorComponent/behaviors/performaces/behaviorPossiblePerformance.h"
#include "engine/aiComponent/behaviorComponent/behaviors/photoTaking/behaviorAestheticallyCenterFaces.h"
#include "engine/aiComponent/behaviorComponent/behaviors/photoTaking/behaviorTakeAPhotoCoordinator.h"
#include "engine/aiComponent/behaviorComponent/behaviors/prDemo/behaviorPRDemo.h"
#include "engine/aiComponent/behaviorComponent/behaviors/prDemo/behaviorPRDemoBase.h"
#include "engine/aiComponent/behaviorComponent/behaviors/proxBehaviors/behaviorProxGetToDistance.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorAskForHelp.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorCheckForAndReactToSalientPoint.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToCliff.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToCubeTap.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToDarkness.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToFrustration.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToHand.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToMicDirection.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToMotion.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToMotorCalibration.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToPlacedOnSlope.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToPutDown.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToRobotOnBack.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToRobotOnFace.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToRobotOnSide.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToRobotShaken.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToSound.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToUncalibratedHeadAndLift.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToUnexpectedMovement.h"
#include "engine/aiComponent/behaviorComponent/behaviors/reactions/behaviorReactToVoiceCommand.h"
#include "engine/aiComponent/behaviorComponent/behaviors/robotDrivenDialog/behaviorPromptUserForVoiceCommand.h"
#include "engine/aiComponent/behaviorComponent/behaviors/sdkBehaviors/behaviorSDKInterface.h"
#include "engine/aiComponent/behaviorComponent/behaviors/sdkBehaviors/behaviorSDKLock.h"
#include "engine/aiComponent/behaviorComponent/behaviors/simpleFaceBehaviors/behaviorDriveToFace.h"
#include "engine/aiComponent/behaviorComponent/behaviors/simpleFaceBehaviors/behaviorFindFaceAndThen.h"
#include "engine/aiComponent/behaviorComponent/behaviors/simpleFaceBehaviors/behaviorLookAtMe.h"
#include "engine/aiComponent/behaviorComponent/behaviors/simpleFaceBehaviors/behaviorSayName.h"
#include "engine/aiComponent/behaviorComponent/behaviors/simpleFaceBehaviors/behaviorSearchWithinBoundingBox.h"
#include "engine/aiComponent/behaviorComponent/behaviors/sleeping/behaviorSleepCycle.h"
#include "engine/aiComponent/behaviorComponent/behaviors/sleeping/behaviorSleeping.h"
#include "engine/aiComponent/behaviorComponent/behaviors/timer/behaviorAdvanceClock.h"
#include "engine/aiComponent/behaviorComponent/behaviors/timer/behaviorDisplayWallTime.h"
#include "engine/aiComponent/behaviorComponent/behaviors/timer/behaviorProceduralClock.h"
#include "engine/aiComponent/behaviorComponent/behaviors/timer/behaviorTimerUtilityCoordinator.h"
#include "engine/aiComponent/behaviorComponent/behaviors/timer/behaviorWallTimeCoordinator.h"
#include "engine/aiComponent/behaviorComponent/behaviors/userDefinedBehaviorTree/behaviorUserDefinedBehaviorSelector.h"
#include "engine/aiComponent/behaviorComponent/behaviors/userDefinedBehaviorTree/behaviorUserDefinedBehaviorTreeRouter.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorConfirmObject.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorPetDetection.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorPoweringRobotOff.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorReactToTouchPetting.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorReactToUnclaimedIntent.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorRobustChargerObservation.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorSnakeGame.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorTrackCube.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorTrackFace.h"
#include "engine/aiComponent/behaviorComponent/behaviors/victor/behaviorWireTest.h"
#include "engine/aiComponent/behaviorComponent/behaviors/volume/behaviorVolume.h"
#include "engine/aiComponent/behaviorComponent/behaviors/weather/behaviorCoordinateWeather.h"
#include "engine/aiComponent/behaviorComponent/behaviors/weather/behaviorDisplayWeather.h"

#include "clad/types/behaviorComponent/behaviorClasses.h"

namespace Anki {
namespace Vector {

ICozmoBehaviorPtr BehaviorFactory::CreateBehavior(const Json::Value& config)
{
  const BehaviorClass behaviorType = ICozmoBehavior::ExtractBehaviorClassFromConfig(config);

  ICozmoBehaviorPtr newBehavior;

  switch (behaviorType)
  {
    case BehaviorClass::GreetAfterLongTime:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorGreetAfterLongTime(config));
      break;
    }
    
    case BehaviorClass::HighLevelAI:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorHighLevelAI(config));
      break;
    }
    
    case BehaviorClass::LookAroundInPlace:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorLookAroundInPlace(config));
      break;
    }
    
    case BehaviorClass::ResetState:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorResetState(config));
      break;
    }
    
    case BehaviorClass::SimpleVoiceResponse:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSimpleVoiceResponse(config));
      break;
    }
    
    case BehaviorClass::StayOnChargerUntilCharged:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorStayOnChargerUntilCharged(config));
      break;
    }
    
    case BehaviorClass::Wait:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorWait(config));
      break;
    }
    
    case BehaviorClass::Alexa:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAlexa(config));
      break;
    }
    
    case BehaviorClass::AlexaSignInOut:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAlexaSignInOut(config));
      break;
    }
    
    case BehaviorClass::AnimGetInLoop:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAnimGetInLoop(config));
      break;
    }
    
    case BehaviorClass::AnimSequence:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAnimSequence(config));
      break;
    }
    
    case BehaviorClass::AnimSequenceWithFace:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAnimSequenceWithFace(config));
      break;
    }
    
    case BehaviorClass::AnimSequenceWithObject:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAnimSequenceWithObject(config));
      break;
    }
    
    case BehaviorClass::CountingAnimation:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorCountingAnimation(config));
      break;
    }
    
    case BehaviorClass::TextToSpeechLoop:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorTextToSpeechLoop(config));
      break;
    }
    
    case BehaviorClass::EyeColor:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorEyeColor(config));
      break;
    }
    
    case BehaviorClass::AttentionTransferIfNeeded:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAttentionTransferIfNeeded(config));
      break;
    }
    
    case BehaviorClass::PickUpCube:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPickUpCube(config));
      break;
    }
    
    case BehaviorClass::PutDownBlock:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPutDownBlock(config));
      break;
    }
    
    case BehaviorClass::PutDownBlockAtPose:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPutDownBlockAtPose(config));
      break;
    }
    
    case BehaviorClass::RollBlock:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorRollBlock(config));
      break;
    }
    
    case BehaviorClass::BumpObject:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorBumpObject(config));
      break;
    }
    
    case BehaviorClass::ClearChargerArea:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorClearChargerArea(config));
      break;
    }
    
    case BehaviorClass::DriveOffCharger:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDriveOffCharger(config));
      break;
    }
    
    case BehaviorClass::FetchCube:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorFetchCube(config));
      break;
    }
    
    case BehaviorClass::FindCube:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorFindCube(config));
      break;
    }
    
    case BehaviorClass::FindCubeAndThen:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorFindCubeAndThen(config));
      break;
    }
    
    case BehaviorClass::FindFaces:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorFindFaces(config));
      break;
    }
    
    case BehaviorClass::FindHome:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorFindHome(config));
      break;
    }
    
    case BehaviorClass::GoHome:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorGoHome(config));
      break;
    }
    
    case BehaviorClass::InteractWithFaces:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorInteractWithFaces(config));
      break;
    }
    
    case BehaviorClass::LookAtFaceInFront:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorLookAtFaceInFront(config));
      break;
    }
    
    case BehaviorClass::MoveHeadToAngle:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorMoveHeadToAngle(config));
      break;
    }
    
    case BehaviorClass::PlaceCubeByCharger:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPlaceCubeByCharger(config));
      break;
    }
    
    case BehaviorClass::PopAWheelie:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPopAWheelie(config));
      break;
    }
    
    case BehaviorClass::RequestToGoHome:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorRequestToGoHome(config));
      break;
    }
    
    case BehaviorClass::Turn:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorTurn(config));
      break;
    }
    
    case BehaviorClass::TurnToFace:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorTurnToFace(config));
      break;
    }
    
    case BehaviorClass::WiggleOntoChargerContacts:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorWiggleOntoChargerContacts(config));
      break;
    }
    
    case BehaviorClass::BlackJack:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorBlackJack(config));
      break;
    }
    
    case BehaviorClass::HowOldAreYou:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorHowOldAreYou(config));
      break;
    }
    
    case BehaviorClass::CoordinateGlobalInterrupts:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorCoordinateGlobalInterrupts(config));
      break;
    }
    
    case BehaviorClass::CoordinateInHabitat:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorCoordinateInHabitat(config));
      break;
    }
    
    case BehaviorClass::CoordinateWhileHeldInPalm:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorCoordinateWhileHeldInPalm(config));
      break;
    }
    
    case BehaviorClass::CoordinateWhileInAir:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorCoordinateWhileInAir(config));
      break;
    }
    
    case BehaviorClass::QuietModeCoordinator:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorQuietModeCoordinator(config));
      break;
    }
    
    case BehaviorClass::VectorPlaysCubeSpinner:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorVectorPlaysCubeSpinner(config));
      break;
    }
    
    case BehaviorClass::DanceToTheBeat:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDanceToTheBeat(config));
      break;
    }
    
    case BehaviorClass::DanceToTheBeatCoordinator:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDanceToTheBeatCoordinator(config));
      break;
    }
    
    case BehaviorClass::ListenForBeats:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorListenForBeats(config));
      break;
    }
    
    case BehaviorClass::CubeDrive:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorCubeDrive(config));
      break;
    }
    
    case BehaviorClass::DevBatteryLogging:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevBatteryLogging(config));
      break;
    }
    
    case BehaviorClass::DevCubeSpinnerConsole:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevCubeSpinnerConsole(config));
      break;
    }
    
    case BehaviorClass::DevDesignCubeLights:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevDesignCubeLights(config));
      break;
    }
    
    case BehaviorClass::DevDisplayReadingsOnFace:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevDisplayReadingsOnFace(config));
      break;
    }
    
    case BehaviorClass::DevEventSequenceCapture:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevEventSequenceCapture(config));
      break;
    }
    
    case BehaviorClass::DevImageCapture:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevImageCapture(config));
      break;
    }
    
    case BehaviorClass::DevSquawkBoxTest:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevSquawkBoxTest(config));
      break;
    }
    
    case BehaviorClass::DevTestBlackjackViz:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevTestBlackjackViz(config));
      break;
    }
    
    case BehaviorClass::DevTestSpriteBoxRemaps:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevTestSpriteBoxRemaps(config));
      break;
    }
    
    case BehaviorClass::DevTouchDataCollection:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevTouchDataCollection(config));
      break;
    }
    
    case BehaviorClass::DevTurnInPlaceTest:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevTurnInPlaceTest(config));
      break;
    }
    
    case BehaviorClass::DevViewCubeBackpackLights:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDevViewCubeBackpackLights(config));
      break;
    }
    
    case BehaviorClass::DispatchAfterShake:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDispatchAfterShake(config));
      break;
    }
    
    case BehaviorClass::DockingTestSimple:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDockingTestSimple(config));
      break;
    }
    
    case BehaviorClass::FactoryCentroidExtractor:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorFactoryCentroidExtractor(config));
      break;
    }
    
    case BehaviorClass::LiftLoadTest:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorLiftLoadTest(config));
      break;
    }
    
    case BehaviorClass::PlannerTest:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPlannerTest(config));
      break;
    }
    
    case BehaviorClass::PowerSaveStressTest:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPowerSaveStressTest(config));
      break;
    }
    
    case BehaviorClass::PowerSaveTest:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPowerSaveTest(config));
      break;
    }
    
    case BehaviorClass::ReactToBody:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToBody(config));
      break;
    }
    
    case BehaviorClass::ReactToGazeDirection:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToGazeDirection(config));
      break;
    }
    
    case BehaviorClass::SelfTest:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTest(config));
      break;
    }
    
    case BehaviorClass::SelfTestButton:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestButton(config));
      break;
    }
    
    case BehaviorClass::SelfTestDockWithCharger:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestDockWithCharger(config));
      break;
    }
    
    case BehaviorClass::SelfTestDriftCheck:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestDriftCheck(config));
      break;
    }
    
    case BehaviorClass::SelfTestDriveForwards:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestDriveForwards(config));
      break;
    }
    
    case BehaviorClass::SelfTestInitChecks:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestInitChecks(config));
      break;
    }
    
    case BehaviorClass::SelfTestLookAtCharger:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestLookAtCharger(config));
      break;
    }
    
    case BehaviorClass::SelfTestMotorCalibration:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestMotorCalibration(config));
      break;
    }
    
    case BehaviorClass::SelfTestPickup:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestPickup(config));
      break;
    }
    
    case BehaviorClass::SelfTestPutOnCharger:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestPutOnCharger(config));
      break;
    }
    
    case BehaviorClass::SelfTestScreenAndBackpack:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestScreenAndBackpack(config));
      break;
    }
    
    case BehaviorClass::SelfTestSoundCheck:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestSoundCheck(config));
      break;
    }
    
    case BehaviorClass::SelfTestTouch:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSelfTestTouch(config));
      break;
    }
    
    case BehaviorClass::DispatcherPassThrough:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDispatcherPassThrough(config));
      break;
    }
    
    case BehaviorClass::DispatcherQueue:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDispatcherQueue(config));
      break;
    }
    
    case BehaviorClass::DispatcherRandom:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDispatcherRandom(config));
      break;
    }
    
    case BehaviorClass::DispatcherRerun:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDispatcherRerun(config));
      break;
    }
    
    case BehaviorClass::DispatcherScoring:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDispatcherScoring(config));
      break;
    }
    
    case BehaviorClass::DispatcherStrictPriority:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDispatcherStrictPriority(config));
      break;
    }
    
    case BehaviorClass::DispatcherStrictPriorityWithCooldown:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDispatcherStrictPriorityWithCooldown(config));
      break;
    }
    
    case BehaviorClass::Exploring:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorExploring(config));
      break;
    }
    
    case BehaviorClass::ExploringExamineObstacle:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorExploringExamineObstacle(config));
      break;
    }
    
    case BehaviorClass::EyeColorVoiceCommand:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorEyeColorVoiceCommand(config));
      break;
    }
    
    case BehaviorClass::LookForFaceAndCube:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorLookForFaceAndCube(config));
      break;
    }
    
    case BehaviorClass::FistBump:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorFistBump(config));
      break;
    }
    
    case BehaviorClass::InspectCube:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorInspectCube(config));
      break;
    }
    
    case BehaviorClass::Keepaway:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorKeepaway(config));
      break;
    }
    
    case BehaviorClass::PounceWithProx:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPounceWithProx(config));
      break;
    }
    
    case BehaviorClass::PuzzleMaze:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPuzzleMaze(config));
      break;
    }
    
    case BehaviorClass::ConnectToCube:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorConnectToCube(config));
      break;
    }
    
    case BehaviorClass::ConfirmHabitat:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorConfirmHabitat(config));
      break;
    }
    
    case BehaviorClass::InitialHeldInPalmReaction:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorInitialHeldInPalmReaction(config));
      break;
    }
    
    case BehaviorClass::ReactToPalmEdge:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToPalmEdge(config));
      break;
    }
    
    case BehaviorClass::KnowledgeGraphQuestion:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorKnowledgeGraphQuestion(config));
      break;
    }
    
    case BehaviorClass::EnrollFace:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorEnrollFace(config));
      break;
    }
    
    case BehaviorClass::RespondToRenameFace:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorRespondToRenameFace(config));
      break;
    }
    
    case BehaviorClass::LeaveAMessage:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorLeaveAMessage(config));
      break;
    }
    
    case BehaviorClass::PlaybackMessage:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPlaybackMessage(config));
      break;
    }
    
    case BehaviorClass::ObservingLookAtFaces:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorObservingLookAtFaces(config));
      break;
    }
    
    case BehaviorClass::ObservingWithoutTurn:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorObservingWithoutTurn(config));
      break;
    }
    
    case BehaviorClass::OnboardingCoordinator:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorOnboardingCoordinator(config));
      break;
    }
    
    case BehaviorClass::OnboardingEmulate1p0WaitForVC:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorOnboardingEmulate1p0WaitForVC(config));
      break;
    }
    
    case BehaviorClass::OnboardingLookAtPhone:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorOnboardingLookAtPhone(config));
      break;
    }
    
    case BehaviorClass::OnboardingLookAtUser:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorOnboardingLookAtUser(config));
      break;
    }
    
    case BehaviorClass::OnboardingTeachWakeWord:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorOnboardingTeachWakeWord(config));
      break;
    }
    
    case BehaviorClass::OnboardingWakeUp:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorOnboardingWakeUp(config));
      break;
    }
    
    case BehaviorClass::PossiblePerformance:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPossiblePerformance(config));
      break;
    }
    
    case BehaviorClass::AestheticallyCenterFaces:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAestheticallyCenterFaces(config));
      break;
    }
    
    case BehaviorClass::TakeAPhotoCoordinator:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorTakeAPhotoCoordinator(config));
      break;
    }
    
    case BehaviorClass::PRDemo:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPRDemo(config));
      break;
    }
    
    case BehaviorClass::PRDemoBase:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPRDemoBase(config));
      break;
    }
    
    case BehaviorClass::ProxGetToDistance:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorProxGetToDistance(config));
      break;
    }
    
    case BehaviorClass::AskForHelp:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAskForHelp(config));
      break;
    }
    
    case BehaviorClass::CheckForAndReactToSalientPoint:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorCheckForAndReactToSalientPoint(config));
      break;
    }
    
    case BehaviorClass::ReactToCliff:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToCliff(config));
      break;
    }
    
    case BehaviorClass::ReactToCubeTap:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToCubeTap(config));
      break;
    }
    
    case BehaviorClass::ReactToDarkness:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToDarkness(config));
      break;
    }
    
    case BehaviorClass::ReactToFrustration:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToFrustration(config));
      break;
    }
    
    case BehaviorClass::ReactToHand:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToHand(config));
      break;
    }
    
    case BehaviorClass::ReactToMicDirection:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToMicDirection(config));
      break;
    }
    
    case BehaviorClass::ReactToMotion:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToMotion(config));
      break;
    }
    
    case BehaviorClass::ReactToMotorCalibration:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToMotorCalibration(config));
      break;
    }
    
    case BehaviorClass::ReactToPlacedOnSlope:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToPlacedOnSlope(config));
      break;
    }
    
    case BehaviorClass::ReactToPutDown:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToPutDown(config));
      break;
    }
    
    case BehaviorClass::ReactToRobotOnBack:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToRobotOnBack(config));
      break;
    }
    
    case BehaviorClass::ReactToRobotOnFace:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToRobotOnFace(config));
      break;
    }
    
    case BehaviorClass::ReactToRobotOnSide:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToRobotOnSide(config));
      break;
    }
    
    case BehaviorClass::ReactToRobotShaken:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToRobotShaken(config));
      break;
    }
    
    case BehaviorClass::ReactToSound:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToSound(config));
      break;
    }
    
    case BehaviorClass::ReactToUncalibratedHeadAndLift:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToUncalibratedHeadAndLift(config));
      break;
    }
    
    case BehaviorClass::ReactToUnexpectedMovement:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToUnexpectedMovement(config));
      break;
    }
    
    case BehaviorClass::ReactToVoiceCommand:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToVoiceCommand(config));
      break;
    }
    
    case BehaviorClass::PromptUserForVoiceCommand:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPromptUserForVoiceCommand(config));
      break;
    }
    
    case BehaviorClass::SDKInterface:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSDKInterface(config));
      break;
    }
    
    case BehaviorClass::SDKLock:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSDKLock(config));
      break;
    }
    
    case BehaviorClass::DriveToFace:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDriveToFace(config));
      break;
    }
    
    case BehaviorClass::FindFaceAndThen:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorFindFaceAndThen(config));
      break;
    }
    
    case BehaviorClass::LookAtMe:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorLookAtMe(config));
      break;
    }
    
    case BehaviorClass::SayName:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSayName(config));
      break;
    }
    
    case BehaviorClass::SearchWithinBoundingBox:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSearchWithinBoundingBox(config));
      break;
    }
    
    case BehaviorClass::SleepCycle:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSleepCycle(config));
      break;
    }
    
    case BehaviorClass::Sleeping:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSleeping(config));
      break;
    }
    
    case BehaviorClass::AdvanceClock:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorAdvanceClock(config));
      break;
    }
    
    case BehaviorClass::DisplayWallTime:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDisplayWallTime(config));
      break;
    }
    
    case BehaviorClass::ProceduralClock:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorProceduralClock(config));
      break;
    }
    
    case BehaviorClass::TimerUtilityCoordinator:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorTimerUtilityCoordinator(config));
      break;
    }
    
    case BehaviorClass::WallTimeCoordinator:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorWallTimeCoordinator(config));
      break;
    }
    
    case BehaviorClass::UserDefinedBehaviorSelector:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorUserDefinedBehaviorSelector(config));
      break;
    }
    
    case BehaviorClass::UserDefinedBehaviorTreeRouter:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorUserDefinedBehaviorTreeRouter(config));
      break;
    }
    
    case BehaviorClass::ConfirmObject:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorConfirmObject(config));
      break;
    }
    
    case BehaviorClass::PetDetection:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPetDetection(config));
      break;
    }
    
    case BehaviorClass::PoweringRobotOff:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorPoweringRobotOff(config));
      break;
    }
    
    case BehaviorClass::ReactToTouchPetting:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToTouchPetting(config));
      break;
    }
    
    case BehaviorClass::ReactToUnclaimedIntent:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorReactToUnclaimedIntent(config));
      break;
    }
    
    case BehaviorClass::RobustChargerObservation:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorRobustChargerObservation(config));
      break;
    }
    
    case BehaviorClass::SnakeGame:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorSnakeGame(config));
      break;
    }
    
    case BehaviorClass::TrackCube:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorTrackCube(config));
      break;
    }
    
    case BehaviorClass::TrackFace:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorTrackFace(config));
      break;
    }
    
    case BehaviorClass::WireTest:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorWireTest(config));
      break;
    }
    
    case BehaviorClass::Volume:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorVolume(config));
      break;
    }
    
    case BehaviorClass::CoordinateWeather:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorCoordinateWeather(config));
      break;
    }
    
    case BehaviorClass::DisplayWeather:
    {
      newBehavior = ICozmoBehaviorPtr(new BehaviorDisplayWeather(config));
      break;
    }
    
  }

  if( ANKI_DEVELOPER_CODE ) {
    newBehavior->CheckJson(config);
  }
    
  return newBehavior;
}

}
}
