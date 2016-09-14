#!/usr/bin/env sh

# Store the start directory for later reference
STARTDIR="$PWD"

# Set this to the number of processor cores you would like to use
NUM_CORES=${NUM_CORES:=1}


# A function to choose the branch to build against
ChooseBranch () {

    echo "Choose the branch you wish to build against:"
    echo ""

    # Store a base counting variable
    num=0

    # Navigate to the makefiles/branch directory
    cd makefiles/branch

    # Loop through all branches supported by the plugin
    for filepath in $(find . -maxdepth 1 -type f -print0 | xargs -0)
    do

        # Increment the counter
        num=$(( $num + 1))

        # Get the name of the file
        filename=${filepath##*/}
        file=${filename%.*}

        # Store the option by its number
        eval option_${num}=${file}

        # Print the current option
        echo -e "\t($num) $file"
    done

    echo ""

    # Request a choice of branch
    read choice

    # Was the choice invalid?
    if ! echo $choice | egrep -q '^[0-9]+$'; then
        ChooseBranch
    elif [ $choice -le 0 ] ; then
        ChooseBranch
    elif [ $choice -gt $num ] ; then
        ChooseBranch

    # The choice was valid
    else

        # Get the branch chosen
        branch=option_$choice
        BRANCH=${!branch}

        # Show the build-type options
        CloneRepo

    fi

}


# A function to create the clone and checkout the selected branch
CloneRepo () {

    # Set the branch's repository directory
    BRANCHDIR="$STARTDIR/hl2sdk/$BRANCH"

    # If the hl2sdk directory does not exist, create the clone
    if [ ! -d "$BRANCHDIR" ]; then
        mkdir -p "$BRANCHDIR"
        cd "$BRANCHDIR"
        git clone https://github.com/alliedmodders/hl2sdk.git .
    else
        cd "$BRANCHDIR"
    fi

    # Try to checkout the selected branch and revert all changes
    if git checkout -f $BRANCH; then
        PullChanges
    else
        echo "Unknown branch to checkout"
    fi
}


# A function to pull repo changes
PullChanges () {

    # Pull the newest changeset from github/alliedmods/hl2sdk
    if git pull; then

        # Move the patched files
        MovePatches

    else

        # Print a message that an error was encountered
        echo "git pull encountered an error."

    fi
}


# A function to move patched files
MovePatches () {

    # Set the branch's patch directory
    PATCHDIR="$STARTDIR/patches/$BRANCH"

    # Copy any patched files over if any exist for the specific branch
    if [ -d "$PATCHDIR" ]; then
        cd "$PATCHDIR"
        cp -r * "$BRANCHDIR"
    fi

    # Create the build files for the branch
    CreateBuild
}


# A function to create build files
CreateBuild () {

    # Navigate back to the starting directory
    cd "$STARTDIR"

    # Set the branch's build directory
    BUILDDIR="$STARTDIR/Builds/Linux/$BRANCH"

    # Does the build directory exist (make it if not)?
    if [ ! -d "$BUILDDIR" ]; then
        mkdir -p "$BUILDDIR"
    fi

    cmake . -B"$BUILDDIR" -DBRANCH=$BRANCH

    # Navigate to the ../Builds/<Branch> directory
    cd "$BUILDDIR"

    # Build the binaries
    make clean
    make -j$NUM_CORES 2>&1 | tee build.log

    # Set the exit code to the exit code of the make command. Otherwise it
    # would be the exit code of the pipe command
    test ${PIPESTATUS[0]} -eq 0

}
if [ "$#" -eq "1" ]; then
    BRANCH=$1
    CloneRepo
else
    ChooseBranch
fi
