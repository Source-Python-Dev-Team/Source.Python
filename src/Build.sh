echo "Enter SDK name: "
read name
if [ ! -d $PWD/Build ]; then
    mkdir $PWD/Build
fi
cd Build
make clean
echo "Build in Debug mode?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) cmake .. -DGAME=$name -DCMAKE_BUILD_TYPE=Debug; break;;
        No ) cmake .. -DGAME=$name; break;;
    esac
done
make
