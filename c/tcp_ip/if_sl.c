static int
slinit(sc)
struct sl_softc *sc;
{
	caddr_t p;
	if(sc->sc_ep == (u_char *)0)
		MCLALLOC(p,M_WAIT);
	if(p)
	{
		sc->sc_ep = (u_char *)p + SLBUFSIZE;
	}
	else{
		//error process
	}
	sc->sc_buf = sc->sc_ep - SLMAX;
	sc->sc_mp = sc->sc_buf;
}

void slinput(c,tp)
int c;
struct tty *tp;
{
	struct sl_softc *sc;
	if((sc = (struct sl_softc *)tp->t_sc) == NULL)
		return;
	if(sc->sc_mp < sc->sc_ep){
		*sc->sc_mp = c;
		sc->sc_mp++;
	}

	switch(c){
		case TRANS_FRAME_ESC:
			if(sc->sc_escape)
				c = FRAME_ESC;
			break;
		case TRANS_FRAME_END:
			if(sc->sc_escape)
				c = FRAME_END;
			break;
		case FRAME_ESC:
			sc->sc_escape = 1;
			return;
		case FRAME_END:
			
	}
}
