doesnthave ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    find * -maxdepth 0 -type d | parallel cd {} \; test -s snp_coords.bed '||' echo {}
}
