headers () 
{ 
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    awk -F, 'NR==1 {for (i=1; i<=NF; i++) { printf("%s\n", $i); }}' /Applications/Database/Postgres.app/Contents/Versions/9.5/share/gdal/vertcs.csv
}
