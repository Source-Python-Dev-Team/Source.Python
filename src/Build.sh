# Store the sdk directory for later reference
SDKDIR=$PWD/sdks

# Store the prefix for sdk directory names
prefix=hl2sdk-

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

    # Was "Release" chosen?
    if [ $choice == "1" ]; then
        cmake .. -DSDK=$name

    # Was "Debug" chosen?
    elif [ $choice == "2" ]; then
        cmake .. -DSDK=$name -DCMAKE_BUILD_TYPE=Debug

    # The choice was invalid
    else
        ChooseBuildType $name

    fi
}

# A function to restart the process from
ChooseSDK () {

    echo "Choose the SDK you wish to build against:"
    echo ""

    # Store a base counting variable
    num=0

    # Loop through all sdks currently downloaded
    for directory in $(find $SDKDIR/* -maxdepth 0 -type d)
    do

        # Increment the counter
        num=$(( $num + 1))

        # Get the name of the directory
        name=${directory##*/}
        name=${name#$prefix}

        # Store the option by its number
        eval option_${num}=${name}

        # Print the current option
        echo -e "\t$num) $name"
    done

    echo ""

    # Request a choice of sdk
    read choice

    # Was the choice invalid?
    if ! echo $choice | egrep -q '^[0-9]+$'; then
        ChooseSDK
    elif [ $choice -le 0 ] ; then
        ChooseSDK
    elif [ $choice -gt $num ] ; then
        ChooseSDK

    # The choice was valid
    else

        # Get the sdk chosen
        name=option_$choice
        name=${!name}

        # Get the build type
        ChooseBuildType $name

    fi

    # Navigate to the Build directory (create it if it does not exist)
    if [ ! -d $PWD/Build ]; then
        mkdir $PWD/Build
    fi
    cd Build

    # Build the binaries
    make clean
    make
}

ChooseSDK
