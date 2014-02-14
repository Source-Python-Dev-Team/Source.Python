echo "Enter SDK name: "
read name
STARTDIR=$PWD
cd sdks
if [ ! -d $PWD/sdks/hl2sdk-$name ]; then
    mkdir $PWD/sdks/hl2sdk-$name
fi
cd hl2sdk-$name
hg init
hg revert --all
hg pull https://hg.alliedmods.net/hl2sdks/hl2sdk-$name
if [ -d $STARTDIR/patches/hl2sdk-$name ]; then
    cp -R $STARTDIR/patches/hl2sdk-$name $STARTDIR/sdks/hl2sdk-$name
fi
