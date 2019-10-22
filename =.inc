= ()
{
    author 'Kevin Ernst'
    about "do math with 'bc' "
    param "$* - 'bc' expression; quote/escape shell metachars"
    example '= 7.68 \* 1024^3'
    group 'math'
    bc -l <<<"$*"
}
