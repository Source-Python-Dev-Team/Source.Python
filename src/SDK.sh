# Store the start directory for later reference
STARTDIR=$PWD

# Store the sdk directory for later reference
SDKDIR=$STARTDIR/makefiles/sdk

# A function to restart the process from
ChooseSDK () {

    echo "Choose the SDK you wish to download:"
    echo ""

    # Store a base counting variable
    num=0

    # Loop through all sdks supported by the plugin
    for filepath in $(find $SDKDIR -maxdepth 1 -type f)
    do

        # Increment the counter
        num=$(( $num + 1))

        # Get the name of the file
        filename=${filepath##*/}
        file=${filename%.*}

        # Store the option by its number
        eval option_${num}=${file}

        # Print the current option
        echo -e "\t$num) $file"
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

        # Navigate to the specific sdk folder (create it if it does not exist)
        cd sdks
        if [ ! -d $PWD/hl2sdk-$name ]; then
            mkdir $PWD/hl2sdk-$name
        fi
        cd hl2sdk-$name

        # Does the hg command exist?
        if hash hg>/dev/null; then

            # Initialize the repo in case this is the first time this sdk is being downloaded
            hg init

            # Pull in any changes
            PullChanges $name

        # Does the command not exist?
        else

            # Print an error message
            echo "Mercurial not found!!  Please install Mercurial."
        fi
    fi
}

PullChanges () {

    # Set the name passed
    name=$1

    # Revert any changes.  This is done to remove any patch changes which are pulled in later.
    hg revert --all

    # Pull the newest changeset from alliedmods.net
    if hg pull -u https://hg.alliedmods.net/hl2sdks/hl2sdk-$name; then

        # Move the patched files
        MovePatches $name

    else

        # Print a message that an error was encountered
        echo "hg pull encountered an error."

    fi
}

MovePatches () {

    # Set the name passed
    name=$1

    # Copy any patched files over if any exist for the specific sdk
    if [ -d $STARTDIR/patches/hl2sdk-$name ]; then
        cd $STARTDIR/patches/hl2sdk-$name
        cp -r * $STARTDIR/sdks/hl2sdk-$name
    fi
}

ChooseSDK
