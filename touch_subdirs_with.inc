touch_subdirs_with ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    lsd | parallel cd {} \; touch -r sub_name.txt . '||' echo '"{}: failed"'
}
