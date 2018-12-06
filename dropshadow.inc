dropshadow ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    convert enabling_online_archive-ann.png -bordercolor white -border 13 \( +clone -background black -shadow 80x3+2+2 \) +swap -background white -layers merge +repage output.png
}
