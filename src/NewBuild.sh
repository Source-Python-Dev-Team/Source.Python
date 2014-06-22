# Store the start directory for later reference
STARTDIR=$PWD


# A function to choose the branch to build against
ChooseBranch () {

    echo "Choose the branch you wish to build against:"
    echo ""

    # Store a base counting variable
    num=0

    # Loop through all branches supported by the plugin
    for filepath in $(find $STARTDIR/makefiles/branch -maxdepth 1 -type f)
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
        ChooseBuildType

    fi

}


# A function to choose the build type
ChooseBuildType () {

    # Print a menu for build type
    echo "Select the build type:"
    echo ""
    echo -e "\t(1) Release"
    echo -e "\t(2) Debug"
    echo ""

    # Request a choice in build type
    read BUILD_TYPE

    # Was the choice invalid?
    if [ ! $BUILD_TYPE == "1" ] && [ ! $BUILD_TYPE == "2" ]; then
        ChooseBuildType
    fi

    # If the choice was valid, create the clone, if it doesn't exist
    CloneRepo

}


# A function to create the clone and checkout the selected branch
CloneRepo () {

    # Set the branch's repository directory
    BRANCHDIR=$STARTDIR/hl2sdk/$BRANCH

    # If the hl2sdk directory does not exist, create the clone
    if [ ! -d $BRANCHDIR ]; then
        mkdir -p $BRANCHDIR
        cd $BRANCHDIR
        git clone https://github.com/alliedmodders/hl2sdk.git .
    else
        cd $BRANCHDIR
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
    PATCHDIR=$STARTDIR/patches/$BRANCH

    # Copy any patched files over if any exist for the specific branch
    if [ -d $PATCHDIR ]; then
        cd $PATCHDIR
        cp -r * $BRANCHDIR
    fi

    # Create the build files for the branch
    CreateBuild
}


# A function to create build files
CreateBuild () {

    # Navigate back to the starting directory
    cd $STARTDIR

    # Set the branch's build directory
    BUILDDIR=$STARTDIR/Builds/$BRANCH

    # Does the build directory exist (make it if not)?
    if [ ! -d $BUILDDIR ]; then
        mkdir -p $BUILDDIR
    fi

    # Building for "Release"?
    if [ $BUILD_TYPE == "1" ]; then
        cmake . -B$BUILDDIR -DBRANCH=$BRANCH

    # Building for "Debug"?
    else
        cmake . -B$BUILDDIR -DBRANCH=$BRANCH -DCMAKE_BUILD_TYPE=Debug

    fi

    # Navigate to the ../Builds/<Branch> directory
    cd $BUILDDIR

    # Build the binaries
    make clean
    make

}

ChooseBranch
