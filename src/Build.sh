echo "Enter SDK name: "
read name
mkdir Build
cd Build
echo "Build in Debug mode?"
select yn in "Yes" "No"; do
    case $yn in
        Yes ) cmake .. -DGame=$name -DCMAKE_BUILD_TYPE=Debug; break;;
        No ) cmake .. -DGame=$name; break;;
    esac
done
