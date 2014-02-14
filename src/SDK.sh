echo "Enter SDK name: "
read name
STARTDIR=$PWD
cd sdks
if [ ! -d $PWD/hl2sdk-$name ]; then
    mkdir $PWD/hl2sdk-$name
fi
cd hl2sdk-$name
hg init
hg revert --all
hg pull -u https://hg.alliedmods.net/hl2sdks/hl2sdk-$name
if [ -d $STARTDIR/patches/hl2sdk-$name ]; then
    cd ..
    cd ..
    cd patches/hl2sdk-$name
    cp -r * $STARTDIR/sdks/hl2sdk-$name
fi
