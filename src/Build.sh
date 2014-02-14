echo "Enter SDK name: "
read name
if [ ! -d $PWD/Build ]; then
    mkdir $PWD/Build
fi
cd Build
make clean
echo "Select the build type: "
select rd in "Release" "Debug"; do
    case $rd in
        Release ) cmake .. -DGAME=$name; break;;
        Debug ) cmake .. -DGAME=$name -DCMAKE_BUILD_TYPE=Debug; break;;
    esac
done
make
