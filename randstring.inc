randstring ()
{
    author 'Kevin Ernst'
    about 'Generate a random string of the specified # of bytes (default 32)'
    about 'source: https://gist.github.com/earthgecko/3089509'
    param '$1 - (optional, default: 32) number of bytes'
    example 'randstring 40  # generate 40-byte token, maybe for an API'
    group 'util'
    cat /dev/urandom | tr -dc 'a-zA-Z0-9' | head -c ${1:-32} 
}
