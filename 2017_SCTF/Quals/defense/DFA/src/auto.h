

struct DFA_node;


struct DFA_rule
{
    char val;
    struct DFA_node* node;
    struct DFA_rule* next;
};

struct DFA_node
{
    int is_final;
    char *name;
    struct DFA_rule* rule;
};

struct DFA_node_iter
{
    struct DFA_node* elem;
    struct DFA_node_iter* next;
};


