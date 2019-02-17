randstring ()
{
    author 'Kevin Ernst'
    about 'Generate a random string of the specified # of bytes (default 32)'
    about 'source: https://gist.github.com/earthgecko/3089509'
    param '$1 - (optional, default: 32) number of bytes'
    param '$2 - (optional, default: /, +, lookalikes) exclude characters'
    example 'randstring 40  # generate 40-byte token, maybe for an API'
    group 'util'
    base64 /dev/urandom | tr -d "${2:-/+O0l1I}" | head -c ${1:-32} 
    echo  # add a final newline, since 'head -c' will remove it
}
