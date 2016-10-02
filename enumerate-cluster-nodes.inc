enumerate-cluster-nodes ()
{
    author 'Kevin Ernst'
    about 'enumerate the number of available compute nodes in the cluster'
    param '(none)'
    example 'enumerate-cluster-nodes'
    group 'lsf'
    bhosts | awk 'BEGIN{c=0}; $2=="ok" {c++}; END{printf("%s\n", c)}'
}
