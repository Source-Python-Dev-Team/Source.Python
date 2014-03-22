# Set the start directory for later reference
STARTDIR=$PWD

# Store the SDK prefix
prefix=hl2sdk-

# A place to locate all currently downloaded SDKs
FindSDKs () {

    # Store a counter
    num=0

    # Loop through all currently downloaded SDKs
    for directory in $(find $STARTDIR/sdks/* -maxdepth 0 -type d)
    do

        # Increment the counter
        num=$(( $num + 1))

        # Get the name of the SDK
        name=${directory##*/}
        name=${name#$prefix}

        # Store the name as the current option
        eval option_${num}=${name}

    done

    # Are no SDKs currently downloaded?
    if [ $num -eq 0 ] ; then
        echo ""
        echo "No SDKs are currently downloaded!"
        echo "You will now be prompted to download an SDK."
        echo ""
        sh SDK.sh
    else
        ChooseSDK
    fi

}

ChooseSDK () {

    echo ""
    echo "Choose the SDK you wish to build against:"
    echo ""

    # Loop through all the SDKs
    for option in $(seq 1 $num)
    do

        # Get the current option
        name=option_$option
        name=${!name}

        # Print the current option
        echo -e "\t($option) $name"

    done

    echo ""

    # Get the SDK chosen to be built against
    read choice

    # Was the choice invalid?
    if ! echo $choice | egrep -q '^[0-9]+$'; then
        ChooseSDK
    elif [ $choice -le 0 ]; then
        ChooseSDK
    elif [ $choice -gt $num ]; then
        ChooseSDK

    # Was the choice valid?
    else

        # Get the SDK chosen
        name=option_$choice
        name=${!name}

        # Show the build-type options
        ChooseBuildType $name

    fi
}

ChooseBuildType () {

    # Get the name of the SDK to build against
    name=$1

    # Print a menu for build type
    echo "Select the build type:"
    echo ""
    echo -e "\t1) Release"
    echo -e "\t2) Debug"
    echo ""

    # Request a choice in build type
    read choice

    # Was the choice invalid?
    if [ ! $choice == "1" ] && [ ! $choice == "2" ]; then
        ChooseBuildType $name
    fi

    # Store the directory to build in
    SDKDIR=$STARTDIR/Builds/$name

    # Does the build directory exist?
    if [ ! -d $SDKDIR ]; then
        mkdir -p $SDKDIR
    fi

    # Building for "Release"?
    if [ $choice == "1" ]; then
        cmake . -B$SDKDIR -DSDK=$name

    # Building for "Debug"?
    else
        cmake . -B$SDKDIR -DSDK=$name -DCMAKE_BUILD_TYPE=Debug

    fi

    # Navigate to the ../Builds/<SDK> directory
    cd Builds/$name

    # Build the binaries
    make clean
    make
}

FindSDKs
