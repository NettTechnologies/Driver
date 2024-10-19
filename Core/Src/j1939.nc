///*
//  Copyright Simma Software, Inc. - 2011
//
//  ssj1939-Full (Version 2.2)
//
//  Use this software at your own risk.  Simma Software, Inc does not promise,
//  state, or guarantee this software to be defect free.
//
//  This file contains software that relates to the implementation of the
//  data-link and network management layer of J1939-21 & J1939-81.
//*/
//#include "stdint.h"
//#include "bits.h"
//#include "can.h"
//#include "j1939.h"
//#include "j1939app.h"
//#include "j1939tp.h"
//#include "j1939cfg.h"
//
//
//
///* wait 250 ms before claiming an address */
//#define J1939_ADDRESS_CLAIM_TIME_OUT            (250U/J1939CFG_TICK_PERIOD)
//
///* apprx number of msgs in 250ms */
//#define J1939_BIP_MSGS_250MS                                           477U
//
///* babble protection window */
//#define J1939_BIP_WINDOW                        (250U/J1939CFG_TICK_PERIOD)
//
//
//
///* babbling-idiot protection variables */
//uint8_t  j1939_bip_time = 0;
//uint16_t j1939_bip_tx_cnt = 0;
//uint16_t j1939_bip_tx_cnt_max = 0;
//uint8_t  j1939_bip_tx_is_disabled = 0;
//uint16_t j1939_bip_tx_cnt_allowed = J1939_BIP_MSGS_250MS/4U;
//
///* address claim algorithm variables */
//uint8_t j1939_address_claim_time = 0;
//uint8_t j1939_address_claim_requested = 0;
//uint8_t j1939_address_claim_has_failed = 0;
//uint8_t j1939_address_claim_tx_requested = 1;
//uint8_t j1939_address_claim_has_been_sent = 0;
//uint8_t j1939_address_claim_was_successful = 0;
//uint8_t j1939_address_cannot_claim_tx_requested = 0;
//uint8_t j1939_address_claim_received_higher_priority = 0;
//
///* source address */
//uint8_t j1939_sa = J1939CFG_SA;
//
//
//
///* j1939 name field */
//uint8_t const
//j1939_name[] = {
//
//  (J1939CFG_N_IN),
//  (J1939CFG_N_IN >> 8),
//  ((uint8_t)((J1939CFG_N_MC << 5) & 0xff) | (J1939CFG_N_IN >> 16)),
//  (J1939CFG_N_MC >> 3),
//  ((J1939CFG_N_FI << 3) | J1939CFG_N_EI),
//  (J1939CFG_N_F),
//  (J1939CFG_N_VS << 1),
//  ((J1939CFG_N_AAC << 7) | (J1939CFG_N_IG << 4) | (J1939CFG_N_VSI))
//};
//
//
//
///*
//** Sets the application's max allowable bus load threshold
//** INPUT: rate - percent load from 0 to 100
//*/
//void
//j1939_bip_tx_rate_allowed_set ( uint8_t rate ) {
//
//  if( rate <= 100 )
//    j1939_bip_tx_cnt_allowed = (J1939_BIP_MSGS_250MS*(uint16_t)rate)/100U;
//
//  return;
//}
//
//
//
///*
//** Returns the application's peak bus load usage
//** RETURN: percent load from 0 to 100
//*/
//uint8_t
//j1939_bip_tx_rate_max_get ( void ) {
//
//  return ((j1939_bip_tx_cnt_max * 100U)/J1939_BIP_MSGS_250MS);
//}
//
//
//
///*
//** Updates the babbling idiot state machine
//*/
//void
//j1939_bip_update ( void ) {
//
//  /* keep track of max */
//  if( j1939_bip_tx_cnt > j1939_bip_tx_cnt_max )
//    j1939_bip_tx_cnt_max = j1939_bip_tx_cnt;
//
//  /* if max is bigger than allowed, then shutdown transmitter */
//  if( j1939_bip_tx_cnt_max > j1939_bip_tx_cnt_allowed )
//    j1939_bip_tx_is_disabled = 1;
//
//  if( ++j1939_bip_time >= J1939_BIP_WINDOW ) {
//
//    j1939_bip_time = 0;
//    j1939_bip_tx_cnt = 0;
//  }
//
//  return;
//}
//
//
//
///*
//** Initializes the entire J1939 protocol stack
//*/
//void
//j1939_init ( void ) {
//
//  j1939tp_init();
//  j1939app_init();
//}
//
//
//
///*
//** This function is the first level of inspection of a J1939 message.  It's
//** sole purpose is to decide where the message should go from here.
//** INPUT: msg - pointer to J1939 message
//*/
//void
//j1939_process ( j1939_t *msg ) {
//
//  uint8_t cnt, tmp_sa;
//  uint32_t rpgn;
//
//  switch( msg->pgn ) {
//
//    /* address claim PGN */
//    case J1939_PGN_ADDRESS_CLAIMED: {
//
//      /* if this message isn't using the same source address, ignore it */
//      if( msg->src != j1939_sa )
//        break;
//
//      /* find first byte which is different (MSB is byte 7) */
//      for( cnt=7; (msg->buf[cnt]==j1939_name[cnt]) && (cnt > 0); cnt-- );
//
//      /* is their's lower (higher priority)? if it's the same NAME
//         we treat it as higher priority and won't transmit */
//      if( msg->buf[cnt] <= j1939_name[cnt] ) {
//
//        /* we can't use the current address anymore */
//        j1939_address_claim_was_successful = 0;
//
//        /* we now know there is a higher priority CA out there */
//        j1939_address_claim_received_higher_priority = 1;
//
//        /* see if the application has a new address for us to try */
//        tmp_sa = j1939app_sa_get();
//
//        /* 254 or 255 indicate no more addresses are available */
//        if( tmp_sa < 254 ) {
//
//          j1939_address_claim_time = 0;
//          j1939_address_claim_tx_requested = 1;
//          j1939_address_cannot_claim_tx_requested = 0;
//          j1939_address_claim_has_been_sent = 0;
//          j1939_address_claim_was_successful = 0;
//          j1939_address_claim_received_higher_priority = 0;
//          j1939_sa = tmp_sa;
//
//        } else {
//
//          /* we failed to claim an address */
//          j1939_address_claim_has_failed = 1;
//
//          /* we can not send a 'cannot claim' without first sending a 'claim' */
//          if( j1939_address_claim_has_been_sent )
//            j1939_address_cannot_claim_tx_requested = 1;
//        }
//
//      } else {
//
//        /* we're high priority, so send another address claim message (only
//           if we haven't already received a higher priority before this) */
//        if( j1939_address_claim_received_higher_priority == 0 )
//          j1939_address_claim_tx_requested = 1;
//      }
//
//      break;
//    }
//
//    /* request */
//    case J1939_PGN_REQUEST: {
//
//      /* find out what pgn is being requested */
//      rpgn = msg->buf[0] | ((uint32_t)msg->buf[1]<<8UL) |
//                           ((uint32_t)msg->buf[2]<<16UL);
//
//      /* if it's a request for address claimed, we handle it here */
//      if( rpgn == J1939_PGN_ADDRESS_CLAIMED ) {
//
//         j1939_address_claim_requested = 1;
//
//      } else {
//
//        j1939app_process( msg );
//      }
//
//      break;
//    }
//
//    /* tp - connection management message */
//    case J1939_PGN_TP_CM:
//      j1939tp_post_cm( msg );
//      break;
//
//    /* tp - data transfer message */
//    case J1939_PGN_TP_DT:
//      j1939tp_post_dt( msg );
//      break;
//
//    /* anything not handled here is handed to application layer */
//    default:
//      j1939app_process( msg );
//      break;
//  }
//
//  return;
//}
//
//
//
///*
//** This function translates a J1939 message into a CAN frame which
//** is then buffered for transmission.
//** INPUT: msg - pointer to J1939 message
//** RETURN: 0 - success
//**         1 - failure
//*/
//uint8_t
//j1939_tx ( j1939_t *msg ) {
//
//  uint8_t cnt, ret;
//  can_t can;
//
//  /* is babbling-idiot protection active? */
//  if( j1939_bip_tx_is_disabled )
//    return 1;
//
//  /* copy over buffer */
//  for( cnt = 0; cnt < msg->buf_len; cnt++ )
//    can.buf[cnt] = msg->buf[cnt];
//
//  /* get size of data to go out */
//  can.buf_len = (uint8_t)msg->buf_len;
//
//  /* we start off by loading the pgn into the CAN frame */
//  can.id = msg->pgn;
//
//  /* is the protocol format less than 240, the ps field is the dst */
//  if( ((uint16_t)msg->pgn) < ((uint16_t)0xf000) )
//    can.id |= msg->dst;
//
//  /* load in the priority */
//  can.id |= (((uint32_t)(msg->pri & 0x7)) << 18UL);
//
//  /* make room for source address and load it in */
//  can.id <<= 8;
//  can.id |= msg->src;
//
//  /* the 31st bit being set indicates its an extended frame message */
//  can.id |= (B31);
//
//  /* send it to the CAN buffer/isr routines */
//  ret = can_tx(&can);
//
//  /* keep track of number of transmits for babble protection */
//  if( ret == 0 )
//    if( j1939_bip_tx_cnt < J1939_BIP_MSGS_250MS )
//      j1939_bip_tx_cnt++;
//
//  return ret;
//}
//
//
//
///*
//** This function buffers a J1939 message for transmission.  For multi-packet
//** messages, *status will be set to J1939TP_INPROCESS during transmission,
//** J1939TP_DONE for success, or J1939TP_FAILED for failure.
//** INPUT: msg - pointer to J1939 message
//**        status - pointer to status variable
//** RETURN: 0 - success
//**         1 - failure
//*/
//uint8_t
//j1939_tx_app ( j1939_t *msg, uint8_t *status ) {
//
//  /* we can not transmit until we have claimed an address */
//  if( j1939_address_claim_was_successful == 0 )
//    return 1;
//
//  /* every message has our source address */
//  msg->src = j1939_sa;
//
//  /* per J1939/21 any global request must also be processed by this ECU.
//     also, the only msg that can be less than 8 bytes is a request */
//  if( msg->pgn == J1939_PGN_REQUEST ) {
//
//    if( msg->dst == J1939_ADDR_GLOBAL )
//      j1939_process( msg );
//
//  } else if( msg->buf_len < 8 ) {
//
//    return 1;
//  }
//
//  /* is it single or multi-packet msg? */
//  if( msg->buf_len <= 8 )
//    return j1939_tx(msg);
//  else
//    return j1939tp_tx(msg,status);
//}
//
//
//
///*
//** Buffers an address claimed message for transmission.
//** INPUT: src - source address of controlling application
//** RETURN: 0 - success
//**         1 - failure
//*/
//uint8_t
//j1939_tx_address_claimed ( uint8_t src ) {
//
//  j1939_t msg;
//
//  /* destination must be global for all address claim msgs. J1939-81 4.2.2 */
//  msg.dst = J1939_ADDR_GLOBAL;
//  msg.src = src;
//  msg.pgn = J1939_PGN_ADDRESS_CLAIMED;
//  msg.pri = 6;
//  msg.buf = (uint8_t *)j1939_name;
//  msg.buf_len = 8;
//
//  return j1939_tx( &msg );
//}
//
//
//
///*
//** Translates a CAN frame into a J1939 message
//*/
//void
//j1939_post ( can_t *can ) {
//
//  j1939_t msg;
//
//  /* if EDP is set, or if it's a 2.0a msg, the message should be ignored */
//  if( (can->id & B25) || ((can->id & B31) == 0) )
//    return;
//
//  /* get source address of frame and chop it off.
//     reference SAE J1939/21 for more information */
//  msg.src = (uint8_t)can->id;
//  can->id >>= 8;
//
//  /* if the protocol formart is less than 240, it is destination specific */
//  if( ((uint8_t)(can->id >> 8)) < 240 ) {
//
//    /* destination specific */
//    msg.dst = (uint8_t)can->id;
//    msg.pgn = can->id & 0x1ff00;
//
//  } else {
//
//    /* ps field is a group extension */
//    msg.dst = J1939_ADDR_GLOBAL;
//    msg.pgn = can->id & 0x1ffff;
//  }
//
//  /* if this packet isn't for us or isn't a broadcast, then ignore */
//  if( (msg.dst != j1939_sa) && (msg.dst != J1939_ADDR_GLOBAL) )
//    return;
//
//  /* J1939_post doesn't own the passed memory so this is ok.  this memory
//     is from the interrupt receive buffer so should be quick and dirty.  */
//  msg.buf = can->buf;
//  msg.buf_len = can->buf_len;
//
//  /* we decoded the CAN frame into the J1939 parts, so process it */
//  j1939_process(&msg);
//}
//
//
//
///*
//** This function updates the address claim state machine.
//*/
//void
//j1939_update_address_claim ( void ) {
//
//  uint8_t tmp;
//
//  /* has an address claim been requested? */
//  if( j1939_address_claim_requested ) {
//
//    /* if we've failed, we send a 'cannot claim', else 'addr claimed' */
//    tmp = j1939_address_claim_has_failed ? J1939_ADDR_NULL : j1939_sa;
//
//    if( j1939_tx_address_claimed(tmp) == 0 )
//      j1939_address_claim_requested = 0;
//  }
//
//  /* has an address claim message been requested? */
//  if( j1939_address_claim_tx_requested &&
//      (j1939_tx_address_claimed(j1939_sa) == 0) ) {
//
//    j1939_address_claim_tx_requested = 0;
//    j1939_address_claim_has_been_sent = 1;
//  }
//
//  /* has a cannot claim message been requested? */
//  if( j1939_address_cannot_claim_tx_requested &&
//      (j1939_tx_address_claimed(J1939_ADDR_NULL) == 0) )
//    j1939_address_cannot_claim_tx_requested = 0;
//
//  /* we wait 250ms after sending out an address claim message.  if we didn't
//     receive a higher priority, we claim the address */
//  if( j1939_address_claim_has_been_sent ) {
//
//    if( j1939_address_claim_time <= J1939_ADDRESS_CLAIM_TIME_OUT ) {
//
//      j1939_address_claim_time++;
//
//    } else if( j1939_address_claim_received_higher_priority == 0 ) {
//
//      j1939_address_claim_was_successful = 1;
//    }
//  }
//
//  return;
//}
//
//
//
///*
//** This function is the time base for the entire J1939 module.
//*/
//void
//j1939_update ( void ) {
//
//  can_t can;
//
//  /* read all the CAN frames are and pass them up */
//  while( can_rx(&can) == 0 )
//    j1939_post(&can);
//
//  /* update address claim state machine */
//  j1939_update_address_claim();
//
//  /* update babbling-idiot protection */
//  j1939_bip_update();
//
//  /* update transport protocol */
//  j1939tp_update();
//
//  /* application layer that transmits messages periodically */
//  j1939app_update();
//
//  return;
//}
