#!/bin/bash

# Full path to the ci/ directory, where this script is located
CI_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"
SRC_DIR=$CI_DIR/..
cd $CI_DIR

JOB_NAME=$1
OPTION=$2
IMAGE_VARIABLE=${JOB_NAME//-/_}__image
LLVM_VERSION_VARIABLE=${JOB_NAME//-/_}__variables__LLVM_VERSION
CROSS_BUILDING_VARIABLE=${JOB_NAME//-/_}__variables__CROSS_BUILDING


# Parse Giltab-ci config file
source yaml.sh
create_variables $SRC_DIR/.gitlab-ci.yml

IMAGE=${!IMAGE_VARIABLE}
LLVM_VERSION=${!LLVM_VERSION_VARIABLE}
LLVM_VERSION=${LLVM_VERSION//\"/}
CROSS_BUILDING=${!CROSS_BUILDING_VARIABLE}
CROSS_BUILDING=${CROSS_BUILDING//\"/}

if [[ -z "$IMAGE" ]]; then
    >&2 echo No valid ci job named \"$JOB_NAME\" found
    exit 1
fi

if [[ ! -z "$CROSS_BUILDING" ]]; then
    CROSS_BUILDING_FLAG="-e CROSS_BUILDING=$CROSS_BUILDING"
fi

if [[ "$OPTION" == "--clean-build" ]]; then
    CLEAN_BUILD_FLAG="-e CLEAN_BUILD=YES"
elif [[ "$OPTION" == "--help" ]]; then
    echo run_job <job-name> [option]
    echo " "
    echo   option:
    echo     --clean-build : Clean buildtree before build
    echo     --help : Show this help
    exit
fi

if [[ -z "$LLVM_VERSION" ]]; then
    >&2 echo LLVM_VERSION variable not set in job $JOB_NAME
    exit 2
fi

echo Running job $JOB_NAME
echo Using docker image $IMAGE
echo CROSS_BUILDING=$CROSS_BUILDING
echo LLVM_VERSION=$LLVM_VERSION

docker run -ti --rm --hostname $JOB_NAME -v $SRC_DIR:/repo -w /repo -e CI_JOB_NAME=$JOB_NAME -e LLVM_VERSION=$LLVM_VERSION $CROSS_BUILDING_FLAG $CLEAN_BUILD_FLAG $IMAGE bash -c "./ci/ci.sh"
