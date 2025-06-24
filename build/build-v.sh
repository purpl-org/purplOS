#!/usr/bin/env bash

TOOLCHAIN_VERSION="5.2.1-r06"

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/.."
cd $DIR

if [[ "$(uname -a)" == *"x86_64"* && "$(uname -a)" == *"Linux"* ]]; then
	HOST="amd64-linux"
	ADEPS="anki-deps"
elif [[ "$(uname -a)" == *"arm64"* && "$(uname -a)" == *"Darwin"* ]]; then
	HOST="arm64-macos"
	ADEPS="$HOME/.anki"
#elif [[ "$(uname -a)" == *"x86_64"* && "$(uname -a)" == *"Darwin"* ]]; then
#        HOST="amd64-macos"
#        ADEPS="$HOME/.anki"
elif [[ "$(uname -a)" == *"aarch64"* && "$(uname -a)" == *"Linux"* ]]; then
        HOST="arm64-linux"
        ADEPS="anki-deps"
else
    echo "This can only be run on x86_64 Linux, arm64 macOS, or arm64 macOS systems at the moment."
    echo "This will be fixed once I compile the new toolchain for more platforms."
    exit 1
fi

echo $HOST

# remove old toolchain
echo "Deleting old 4.0.0-r05 toolchain if it exists..."
rm -rf $ADEPS/vicos-sdk/dist/4.0.0-r05

if [[ ! -d "$ADEPS/vicos-sdk/dist/${TOOLCHAIN_VERSION}/prebuilt" ]]; then
	mkdir -p "$ADEPS/vicos-sdk/dist/${TOOLCHAIN_VERSION}"
	cd "$ADEPS/vicos-sdk/dist/${TOOLCHAIN_VERSION}"
	wget -q --show-progress https://github.com/os-vector/wire-os-externals/releases/download/${TOOLCHAIN_VERSION}/vicos-sdk_${TOOLCHAIN_VERSION}_$HOST.tar.gz
	tar -zxf vicos-sdk_${TOOLCHAIN_VERSION}_$HOST.tar.gz
	rm -f vicos-sdk_${TOOLCHAIN_VERSION}_$HOST.tar.gz
fi

cd "$DIR"

if [[ ! -d "$ADEPS/wwise" ]]; then
	mkdir -p "$ADEPS/wwise/versions/2017.2.7_a"
	cd "$ADEPS/wwise/versions/2017.2.7_a"
	wget -q --show-progress https://github.com/os-vector/wire-os-externals/releases/download/4.0.0-r05/wwise-2017.2.7_a.tar.gz
	tar -zxf wwise-2017.2.7_a.tar.gz
	rm -f wwise-2017.2.7_a.tar.gz
fi

cd "$DIR"

if [[ "$(uname -a)" == *"Darwin"* ]]; then
	echo "Installing required Go stuff..."
	go install google.golang.org/protobuf/cmd/protoc-gen-go@latest
	go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@latest
	go install github.com/grpc-ecosystem/grpc-gateway/v2/protoc-gen-grpc-gateway@latest
	export PATH="$PATH:$HOME/go/bin"
	./project/victor/scripts/victor_build_release.sh "$@"
	echo
	echo -e "\033[1;32mComplete.\033[0m"
	echo
else
	mkdir -p build/cache
	mkdir -p build/gocache
	mkdir -p build/usercache
	if [[ ! -z $(docker images -q vic-standalone-builder-5) ]]; then
		echo "Purging old vic-standalone-builder-5 container... this might take a while"
		docker ps -a --filter "ancestor=vic-standalone-builder-5" -q | xargs -r docker rm -f
		docker rmi -f $(docker images --filter "reference=vic-standalone-builder-5*" --format '{{.ID}}')
		#echo
		#echo -e "\033[5m\033[1m\033[31mOld Docker builder detected on system. If you have built victor or wire-os many times, it is recommended you run:\033[0m"
		#echo
		#echo -e "\033[1m\033[36mdocker system prune -a --volumes\033[0m"
		#echo
		#echo -e "\033[32mContinuing in 5 seconds... (you will only see this message once)\033[0m"
		#sleep 5
	fi
	if [[ -z $(docker images -q vic-standalone-builder-6) ]]; then
		docker build \
		--build-arg DIR_PATH="$(pwd)" \
		--build-arg USER_NAME=$USER \
		--build-arg UID=$(id -u $USER) \
		--build-arg GID=$(id -u $USER) \
		-t vic-standalone-builder-6 \
		build/
	else
		echo "Reusing vic-standalone-builder-6"
	fi
	docker run --rm -it \
		-v $(pwd)/anki-deps:/home/$USER/.anki \
		-v $(pwd):$(pwd) \
		-v $(pwd)/build/cache:/home/$USER/.ccache \
		-v $(pwd)/build/gocache:/home/$USER/go \
		-v $(pwd)/build/usercache:/home/$USER/.cache \
		vic-standalone-builder-6 bash -c \
		"cd $(pwd) && \
		./project/victor/scripts/victor_build_release.sh $@ && \
		echo && \
		echo -e \"\e[1;32mComplete.\e[0m\" && \
		echo"
fi
