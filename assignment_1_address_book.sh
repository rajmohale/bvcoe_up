name=-1
city=-1
code=-1
mobno=-1
n=0
ch=0

while [ $ch -ne 7 ]
do
echo "\n\tAddress Database\n\t-----------------"
echo "1. Create Database\n2. View Database\n3. Insert a Record\n4. Delete a Record"
echo "5. Search a Record\n6. Modify a Record\n7. Exit"
echo "Enter your Choice: "
read ch

case $ch in
1) # Create Database
echo "\nEnter the filename:"
read fname
touch "$fname"

echo "Enter the number of records to be entered in file $fname:"
read n
i=$n

while [ $i -gt 0 ]
do
  echo "\nEnter name:"
  read name
  echo "Enter City:"
  read city
  echo "Enter Zipcode:"
  read code
  echo "Enter Mobile no.:"
  read mobno
  echo "$name\t$city\t$code\t$mobno" >> "$fname"
  echo "Your data is stored in the file."
  i=$((i-1))
  done
  ;;

2) # View Database
if [ -f "$fname" ]; then
  echo "\nName\tCity\tZipcode\tMobile_Number"
  cat "$fname"
else
  echo "\nFile not found. Create database first!"
fi
;;

3) # Insert Record
echo "\nEnter name:"
read name
echo "Enter City:"
read city
echo "Enter Zipcode:"
read code
echo "Enter Mobile no.:"
read mobno
echo "$name\t$city\t$code\t$mobno" >> "$fname"
echo "Record inserted successfully."
;;

4) # Delete Record
echo "\nEnter the name of the record to be deleted:"
read no
if grep -w "$no" "$fname" > /dev/null; then
  grep -w -v "$no" "$fname" > temp
  mv temp "$fname"
  echo "Record deleted successfully!"
else
  echo "Record not found."
fi
;;

5) # Search Record
echo "\nEnter the name of the record to be searched:"
read no
if grep -w "$no" "$fname" > /dev/null; then
  grep -w "$no" "$fname"
else
  echo "Record not found."
fi
;;

6) # Modify Record
echo "\nEnter the name of the record to be modified:"
read name
if grep -w "$name" "$fname" > /dev/null; then
  grep -w -v "$name" "$fname" > temp
  echo -e "Enter new name:"
  read name
  echo "Enter new City:"
  read city
  echo "Enter new Zipcode:"
  read code
  echo "Enter new Mobile no.:"
  read mobno
  echo "$name\t$city\t$code\t$mobno" >> temp
  mv temp "$fname"
  echo "Record modified successfully."
else
  echo "Record not found!"
fi
;;

7) # Exit
echo "\nExiting the program. Goodbye!"
;;

*) echo "\nInvalid choice. Please enter a number between 1 and 7."
 ;;
  esac
done
