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
        branch=${!branch}

        # Show the build-type options
        ChooseBuildType $branch

    fi

}


# A function to choose the build type
ChooseBuildType () {

    # Get the name of the branch to build agains
    branch=$1

    # Print a menu for build type
    echo "Select the build type:"
    echo ""
    echo -e "\t(1) Release"
    echo -e "\t(2) Debug"
    echo ""

    # Request a choice in build type
    read build_type

    # Was the choice invalid?
    if [ ! $build_type == "1" ] && [ ! $build_type == "2" ]; then
        ChooseBuildType $branch
    fi

    # If the choice was valid, create the clone, if it doesn't exist
    CloneRepo $branch $build_type

}


# A function to create the clone and checkout the selected branch
CloneRepo () {

    # Set the variables passed
    branch=$1
    build_type=$2

    # If the hl2sdk directory does not exist, create the clone
    if [ ! -d $STARTDIR/hl2sdk ]; then
        mkdir $STARTDIR/hl2sdk
        cd hl2sdk
        git clone https://github.com/alliedmodders/hl2sdk.git .
    else
        cd hl2sdk
    fi

    # Try to checkout the selected branch and revert all changes
    if git checkout -f $branch; then
        PullChanges $branch $build_type
    else
        echo "Unknown branch to checkout"
    fi
}


# A function to pull repo changes
PullChanges () {

    # Set the variables passed
    branch=$1
    build_type=$2

    # Pull the newest changeset from github/alliedmods/hl2sdk
    if git pull; then

        # Move the patched files
        MovePatches $branch $build_type

    else

        # Print a message that an error was encountered
        echo "git pull encountered an error."

    fi
}


# A function to move patched files
MovePatches () {

    # Set the variables passed
    branch=$1
    build_type=$2

    # Copy any patched files over if any exist for the specific branch
    if [ -d $STARTDIR/patches/hl2sdk-$ ]; then
        cd $STARTDIR/patches/hl2sdk-$
        cp -r * $STARTDIR/hl2sdk
    fi

    # Create the build files for the branch
    CreateBuild $branch $build_type
}


# A function to create build files
CreateBuild () {

    # Set the variables passed
    branch=$1
    build_type=$2

    # Navigate back to the starting directory
    cd $STARTDIR

    # Get the branch's build directory
    BUILDDIR=$STARTDIR/Builds/$branch

    # Does the build directory exist (make it if not)?
    if [ ! -d $BUILDDIR ]; then
        mkdir -p $BUILDDIR
    fi

    # Building for "Release"?
    if [ $choice == "1" ]; then
        cmake . -B$BUILDDIR -DBRANCH=$branch

    # Building for "Debug"?
    else
        cmake . -B$BUILDDIR -DBRANCH=$branch -DCMAKE_BUILD_TYPE=Debug

    fi

    # Navigate to the ../Builds/<Branch> directory
    cd Builds/$branch

    # Build the binaries
    make clean
    make

}

ChooseBranch
