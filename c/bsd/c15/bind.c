struct bind_args{
	int s;
	caddr_t name;
	int namelen; 
};

bind(p,uap,retval)
struct proc *p;
struct bind_args *uap;
int *retval;
{
	int error;
	struct file *fp;
	struct mbuf *nam;
	if(error=getsock(p->p_fd,uap->s,&fp)){
		return error;
	}
	if(error=sockargs(&nam,uap->name,uap->namelen,MT_SONAME)){
		return error;
	}
	error=sobind((struct socket *)fp->f_data,nam);
	m_freem(nam);
	return error;
}

sobind(so,nam)
struct socket *so;
struct mbuf *nam;
{
	int s=splnet();
	int error;
	error=(*so->so_proto->pr_usrreq)(so,PRU_BIND,(struct mbuf *)0,nam,(struct mbuf *)0);
	splx(s);
	return error;
}


getsock(fdp,fds,fpp)
struct filedesc *fdp;
int fds;
struct file **fpp;
{
	struct file *fp;
	if(fds>=fdp->fd_nfiles || (fp=fdp->fd_ofiles[fds])==NULL)
		return (EBADF);
	if(fp->f_type!=DTYPE_SOCKET)
		return (ENOTSOCK);
	*fpp=fp;
	return 0;
}

sockargs(mp,buf,buflen,type)
struct mbuf **mp;
caddr_t buf;
int buflen;
int type;
{
	struct mbuf *m;
	int error;
	struct sockaddr *sa;
	if((u_int)buflen>MLEN){
		return (EINVAL);
	}
	m=mget(M_WAIT,type);
	if(m==NULL)
	{
		return (ENOBUFS);
	}
	m->m_len=buflen;
	error=copyin(buf,mtod(m,caddr_t),(u_int)buflen);
	if(error)
		(void)m_free(m);
	else{
		if(type==MT_SONAME)
		{
			sa=mtod(m,struct sockaddr *);
			sa->sa_len=buflen;
		}
	}
	return error;
}

struct listen_args{
	int s;
	int backlog;
};
listen(p,uap,retval)
struct proc *p;
struct listen_args *uap;
int *retval;
{
	int error;
	struct file *fp;
	if(error=getsock(p->p_fd,uap->s,&fp))
		return error;
	return (solisten((struct socket *)fp->f_data,uap->backlog));
}

solisten(so,backlog)
struct socket *so;
int backlog;
{
	int error;
	int s=splnet();
	error=(*so->so_proto->pr_usrreq)(so,PRU_LISTEN,(struct mbuf *)0,(struct mbuf *)0,(struct mbuf *)0);
	if(error)
	{
		splx(s);
		return error;
	}
	if(so->so_q == 0)
	{
		so->so_options |= SO_ACCEPTCONN;
	}
	if(backlog<0)
		backlog=0;	
	so->so_qlimit=min(backlog,SOMAXCONN);
	splx(s);
	return (0);
}






















