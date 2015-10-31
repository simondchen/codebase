struct iovec{
	void *iov_base;
	size_t iov_len;
};

struct msghdr{
	caddr_t msg_name;
	u_int msg_namelen;
	struct iovec *msg_iov;
	u_int msg_iovlen;
	caddr_t msg_control;
	int mssg_controlen;
	int msg_flags;
};

struct sendmsg_args{
	int s;
	caddr_t msg;
	int flag;
}
sendmsg(p,uap,retval)
struct proc *p;
struct sendmsg_args *uap;
int *retval;
{
	int error;
	struct file *fp;
	struct socket *so;
	struct mbuf *m;
	if(error=getsock(p->p_fd,uap->s,&fp))
		return error;
	//if(error=sockargs(&m,uap->msg,))

	struct msghdr msg;
	struct iovec aio[UIO_SMALLIOV],*iov;
	//copy msg struct to msg here,but pay attention,the msg_iov points to the buffer in userspace!!!
	if(error=copyin(uap->msg,(caddr_t)msg.msg_name,sizeof(msg))){
		return error;
	}
	//not use sockargs directly because we have to check the params firstly
	if((u_int)msg.msg_iovlen>=UIO_SMALLIOV){
		if((u_int)msg.msg_iovlen>=UIO_MAXIOV)
		{
			return EMSGSIZE;
		}
		MALLOC(iov,struct iovec,sizeof(struct iovec)*(u_int)msg.msg_iovlen,M_IOV,M_WAITOK);
	}else{
		iov=aio;
	}
	if(msg.msg_iovlen && error=)
}

sendit()
struct proc *p;
struct msghdr *mp;
{
	struct mbuf *to;
	struct uio auio;
	struct iovec *iov;
	int i;
	auio.uio_iov=mp->msg_iov;
	auio.uio_iovcnt=mp->msg_iovlen;
	auio.uio_offset=0;
	auio.uio_segflg=UIO_USERSPACE
	auio.uio_rw=UIO_WRITE
	auio.uio_procp=p;
	iov=auio.uio_iov;
	for(i=0;i<mp->msg_iovlen;i++,iov++)
	{
		if(iov->iov_len<0)
			return EINVAL;
		if((auio.uio_resid+=iov->iov_len)<0)
			return EINVAL;
	}
	sockargs(&to,mp->msg_name,mp->msg_namelen,MT_SONAME);	
}
