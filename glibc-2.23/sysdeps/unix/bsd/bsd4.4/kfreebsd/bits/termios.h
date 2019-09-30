/* termios type and macro definitions.  FreeBSD version.
   Copyright (C) 1993-1994,1996-1997,1999,2001-2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _TERMIOS_H
# error "Never include <bits/termios.h> directly; use <termios.h> instead."
#endif

/* These macros are also defined in some <bits/ioctls.h> files (with
   numerically identical values), but this serves to shut up cpp's
   complaining. */
#ifdef __USE_MISC

# ifdef MDMBUF
#  undef MDMBUF
# endif
# ifdef FLUSHO
#  undef FLUSHO
# endif
# ifdef PENDIN
#  undef PENDIN
# endif

#endif /* __USE_MISC */

#ifdef ECHO
# undef ECHO
#endif
#ifdef TOSTOP
# undef TOSTOP
#endif
#ifdef NOFLSH
# undef NOFLSH
#endif


typedef unsigned char	cc_t;
typedef unsigned int	speed_t;
typedef unsigned int	tcflag_t;

#define NCCS 20
struct termios
  {
    tcflag_t c_iflag;		/* input mode flags */
    tcflag_t c_oflag;		/* output mode flags */
    tcflag_t c_cflag;		/* control mode flags */
    tcflag_t c_lflag;		/* local mode flags */
    cc_t c_cc[NCCS];		/* control characters */
    speed_t c_ispeed;		/* input speed */
    speed_t c_ospeed;		/* output speed */
#define __ispeed c_ispeed
#define __ospeed c_ospeed
#define _HAVE_STRUCT_TERMIOS_C_ISPEED 1
#define _HAVE_STRUCT_TERMIOS_C_OSPEED 1
  };

/* c_cc characters */
#define VEOF 0
#define VEOL 1
#define VEOL2 2
#define VERASE 3
#define VWERASE 4
#define VKILL 5
#define VREPRINT 6
#ifdef __USE_MISC
# define VERASE2 7
#endif
#define VINTR 8
#define VQUIT 9
#define VSUSP 10
#ifdef __USE_MISC
# define VDSUSP 11
#endif
#define VSTART 12
#define VSTOP 13
#define VLNEXT 14
#define VDISCARD 15
#define VMIN 16
#define VTIME 17
#ifdef __USE_MISC
# define VSTATUS 18
#endif

/* c_iflag bits */
#define IGNBRK	0000001
#define BRKINT	0000002
#define IGNPAR	0000004
#define PARMRK	0000010
#define INPCK	0000020
#define ISTRIP	0000040
#define INLCR	0000100
#define IGNCR	0000200
#define ICRNL	0000400
#define IXON	0001000
#define IXOFF	0002000
#define IXANY	0004000
#define IMAXBEL	0020000

/* c_oflag bits */
#define	OPOST	(1 << 0)	/* Perform output processing.  */
#define	ONLCR	(1 << 1)	/* Map NL to CR-NL on output.  */
#if defined __USE_MISC || defined __USE_XOPEN
# define TAB0   (0 << 2)	/* no tab delay and expansion */
# define TAB3   (1 << 2)	/* expand tabs to spaces */
# define TABDLY	TAB3		/* tab delay mask */
# define OXTABS	TAB3
# define XTABS	TAB3
#endif
#ifdef	__USE_MISC
# define ONOEOT	(1 << 3)	/* Discard EOT (^D) on output.  */
#endif
#define	OCRNL	(1 << 4)	/* map CR to NL on output */
#define	ONOCR	(1 << 5)	/* no CR output at column 0 */
#define	ONLRET	(1 << 6)	/* NL performs CR function */

/* c_cflag bit meaning */
#ifdef	__USE_MISC
# define CIGNORE	(1 << 0)	/* Ignore these control flags.  */
#endif
#define	CSIZE	(CS5|CS6|CS7|CS8)	/* Number of bits per byte (mask).  */
#define	CS5	(0 << 8)	/* 5 bits per byte.  */
#define	CS6	(1 << 8)	/* 6 bits per byte.  */
#define	CS7	(2 << 8)	/* 7 bits per byte.  */
#define	CS8	(3 << 8)	/* 8 bits per byte.  */
#define	CSTOPB	(1 << 10)	/* Two stop bits instead of one.  */
#define	CREAD	(1 << 11)	/* Enable receiver.  */
#define	PARENB	(1 << 12)	/* Parity enable.  */
#define	PARODD	(1 << 13)	/* Odd parity instead of even.  */
#define	HUPCL	(1 << 14)	/* Hang up on last close.  */
#define	CLOCAL	(1 << 15)	/* Ignore modem status lines.  */
#ifdef	__USE_MISC
# define CCTS_OFLOW	(1 << 16)	/* CTS flow control of output.  */
# define CRTS_IFLOW	(1 << 17)	/* RTS flow control of input.  */
# define CRTSCTS	(CCTS_OFLOW|CRTS_IFLOW)	/* CTS/RTS flow control.  */
# define CDTR_IFLOW	(1 << 18)	/* DTR flow control of input.  */
# define CDSR_OFLOW	(1 << 19)	/* DSR flow control of output.  */
# define CCAR_OFLOW	(1 << 20)	/* DCD flow control of output.  */
# define MDMBUF		(1 << 20)	/* Carrier flow control of output.  */
#endif

/* c_lflag bits */
#ifdef	__USE_MISC
# define ECHOKE	(1 << 0)	/* Visual erase for KILL.  */
#endif
#define	ECHOE	(1 << 1)	/* Visual erase for ERASE.  */
#define	ECHOK	(1 << 2)	/* Echo NL after KILL.  */
#define	ECHO	(1 << 3)	/* Enable echo.  */
#define	ECHONL	(1 << 4)	/* Echo NL even if ECHO is off.  */
#ifdef	__USE_MISC
# define ECHOPRT	(1 << 5)	/* Hardcopy visual erase.  */
# define ECHOCTL	(1 << 6)	/* Echo control characters as ^X.  */
#endif
#define	ISIG	(1 << 7)	/* Enable signals.  */
#define	ICANON	(1 << 8)	/* Do erase and kill processing.  */
#ifdef	__USE_MISC
# define ALTWERASE (1 << 9)	/* Alternate WERASE algorithm.  */
#endif
#define	IEXTEN	(1 << 10)	/* Enable DISCARD and LNEXT.  */
#ifdef __USE_MISC
# define EXTPROC (1 << 11)	/* External processing.  */
#endif
#define	TOSTOP	(1 << 22)	/* Send SIGTTOU for background output.  */
#ifdef	__USE_MISC
# define FLUSHO	(1 << 23)	/* Output being flushed (state).  */
# define NOKERNINFO (1 << 25)	/* Disable VSTATUS.  */
# define PENDIN	(1 << 29)	/* Retype pending input (state).  */
#endif
#define	NOFLSH	(1 << 31)	/* Disable flush after interrupt.  */

  /* Input and output baud rates.  */
#define	B0	0		/* Hang up.  */
#define	B50	50		/* 50 baud.  */
#define	B75	75		/* 75 baud.  */
#define	B110	110		/* 110 baud.  */
#define	B134	134		/* 134.5 baud.  */
#define	B150	150		/* 150 baud.  */
#define	B200	200		/* 200 baud.  */
#define	B300	300		/* 300 baud.  */
#define	B600	600		/* 600 baud.  */
#define	B1200	1200		/* 1200 baud.  */
#define	B1800	1800		/* 1800 baud.  */
#define	B2400	2400		/* 2400 baud.  */
#define	B4800	4800		/* 4800 baud.  */
#define	B9600	9600		/* 9600 baud.  */
#define	B19200	19200		/* 19200 baud.  */
#define	B38400	38400		/* 38400 baud.  */
#define B76800	76800
#ifdef	__USE_MISC
# define EXTA	19200
# define EXTB	38400
#endif
#define B7200	7200
#define B14400	14400
#define B28800	28800
#define	B57600	57600
#define	B115200	115200
#define	B230400	230400
#define	B460800	460800
#define	B500000	500000
#define	B576000	576000
#define	B921600	921600
#define	B1000000 1000000
#define	B1152000 1152000
#define	B1500000 1500000
#define	B2000000 2000000
#define	B2500000 2500000
#define	B3000000 3000000
#define	B3500000 3500000
#define	B4000000 4000000
#define	__MAX_BAUD B4000000

/* tcflow() and TCXONC use these */
#define	TCOOFF		1
#define	TCOON		2
#define	TCIOFF		3
#define	TCION		4

/* tcflush() and TCFLSH use these */
#define	TCIFLUSH	1
#define	TCOFLUSH	2
#define	TCIOFLUSH	3

/* tcsetattr uses these */
#define	TCSANOW		0
#define	TCSADRAIN	1
#define	TCSAFLUSH	2
#ifdef	__USE_MISC
# define TCSASOFT	0x10	/* Flag: Don't alter hardware state.  */
#endif

