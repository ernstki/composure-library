divide ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    split -dl $(( $(wc -l seq.txt | awk '{print $1}') / 50 )) seq.txt seq-
}
