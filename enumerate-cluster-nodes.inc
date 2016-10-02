enumerate-cluster-nodes ()
{
    author 'Kevin Ernst'
    about ''
    param ''
    example ''
    group ''
    bhosts | awk 'BEGIN{c=0}; $2=="ok" {c++}; END{printf("%s\n", c)}'
}
