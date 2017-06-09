tablook ()
{
    author 'Kevin Ernst'
    about 'Pretty-print a tab-delimited file'
    param '$1 - tab-delimited file name'
    example 'tablook file1.tab'
    group ''
    csvlook -d $'\t' "$1"
}
