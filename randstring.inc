randstring ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    cat /dev/urandom | tr -dc 'a-zA-Z0-9' | head -c ${1:-32} | xclip -i -sel clipboard
}
