//不可以声明为static,否则在外部文件中无法链接
void err_sys(const char *,...);
void perr_exit(const char *msg);
