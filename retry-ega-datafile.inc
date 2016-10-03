retry-ega-datafile ()
{
    author 'Kevin Ernst'
    about 'Repeat a failed download from ftp://xfer.crg.eu'
    param '$1 - the pathname (beginning at /) of the file to re-download'
    example ''
    group 'ega'
    curl -n --limit-rate 200k -o "$1" ftp://xfer.crg.eu/$1
}
