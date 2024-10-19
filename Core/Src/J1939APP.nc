/*
  Copyright Simma Software, Inc. - 2008

  Use this software at your own risk.  Simma Software, Inc does not promise,
  state, or guarantee this software to be defect free.

  This is an example application layer routine for J1939.  For more information
  see J1939-71.

  NOTE:
  1) for multibyte parameters that appear in the data field of a CAN data frame
     they shall be placed least significant byte first. (J1939/21)
*/
#include "j1939app.h"



uint16_t j1939app_engine_speed = 0;



/*
** Initialize application layer
*/
void
j1939app_init ( void ) {

  /* set babbling-idiot protection to 25% */
  j1939_bip_tx_rate_allowed_set(25);

  return;
}



/*
** When the J1939 protocol stack module fails to claim it's default address,
** as defined by J1939CFG_SA in j1939cfg.h, it will need a new address to
** claim.  Since the protocol stack has no knowledge of what additional
** addresses it may claim, it calls this routine so the application layer
** can provide the next address to try.
** RETURN: an appropriate source address from 128 to 248 (inclusive).
**         255 indicates there are no more addresses to try
*/
uint8_t
j1939app_sa_get ( void ) {

  static uint8_t j1939app_arb_addr_cnt = 127;

  /* see j1939-81, section 3.3.2 for more details */
  if( j1939app_arb_addr_cnt <= 247 )
     j1939app_arb_addr_cnt++;
  else
     j1939app_arb_addr_cnt = 255;

  return j1939app_arb_addr_cnt;
}



/*
** This function transmits a J1939 request message.  Use this for retrieving
** request based PGNs (e.g. Vehicle Identification).  To send the request to
** all ECUs on the network, set dst to 255.
** INPUT: rpgn - requested PGN
**        dst - destination address
** RETURN: 0 - success
**         1 - failure
*/
uint8_t
j1939app_tx_request ( uint32_t rpgn, uint8_t dst ) {

  j1939_t msg;
  uint8_t buf[8];

  msg.buf = buf;
  msg.pri = 6;
  msg.pgn = J1939_PGN_REQUEST;
  msg.buf_len = 3;
  msg.dst = dst;
  msg.buf[0] = (uint8_t)rpgn;
  msg.buf[1] = (uint8_t)(rpgn >> 8);
  msg.buf[2] = (uint8_t)(rpgn >> 16);

  return j1939_tx_app( &msg, 0 );
}



/*
** This is where the application code should receive J1939 messages (PGNs).
** All messages which are not used by the data-link and network portions of
** the stack are posted here.  To add new msgs, add the additinal PGNs to 
** the below switch statement.
** INPUT: msg - pointer to received J1939 message (PGN)
*/
void
j1939app_process ( j1939_t *msg ) {

  switch( msg->pgn ) {

    /* get the engine speed */
    case 61444: {

      /* engine speed (SPN 190) */
      j1939app_engine_speed = ((uint16_t)msg->buf[4] << 8) | msg->buf[3];

      break;
    }
  }

  return;
}



/*
** This function is called by the j1939.c at the system tick rate and is
** used to provide a time base to the application layer code.  This time base
** can be used to periodically broadcast PGNs or to indentify when a received
** PGN has timed out.
*/
void
j1939app_update ( void ) {

  return;
}
