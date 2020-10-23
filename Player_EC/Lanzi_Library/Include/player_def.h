// identificazione univoca eventi player DA
#define ID_EVENT_NULL                     0x00  // nessun codice
#define ID_EVENT_BADGE_LETTURA            0x01  // avvenuta lettura badge
#define ID_EVENT_START_PROC               0x02  // inizio procedura verifica badge
#define ID_EVENT_BADGE_NON_PREVISTA       0x03  // codice risposta non previsto
#define ID_EVENT_BADGE_ABILITATO          0x04  // badge abilitato
#define ID_EVENT_BADGE_SCONOSCIUTO        0x05  // badge sconosciuto
#define ID_EVENT_BADGE_CESSATO            0x06  // badge cessato
#define ID_EVENT_BADGE_NON_ANCORA_ATTIVO  0x07  // badge non ancora attivo
#define ID_EVENT_BADGE_NON_ABILITATO      0x08  // badge non abilitato
#define ID_EVENT_BADGE_FUORI_ORARIO       0x09  // badge fuori orario
#define ID_EVENT_BADGE_CARICATORE         0x0A  // badge caricatore
#define ID_EVENT_BADGE_GET_NEW            0x0B  // inserimento nuovo badge
#define ID_EVENT_BADGE_DISPLAY_NEW        0x0C  // visualizza nuovo badge
#define ID_EVENT_LIMITE_MAX_RAGG          0x0D  // limite massimo raggiunto
#define ID_EVENT_PRODOTTO_NON_ABILITATO   0x0E  // prodotto non abilitato
#define ID_EVENT_PRELIEVO_NON_AUTORIZZATO 0x0F  // prelievo non autorizzato
#define ID_EVENT_PRELIEVO_REGOLARE        0x10  // prelievo regolare
#define ID_EVENT_PRELIEVO_AUTORIZZATO     0x11  // prelievo autorizzato
#define ID_EVENT_PRELIEVO_ANNULLATO       0x12  // prelievo annullato
#define ID_EVENT_PRELIEVO_ESEGUITO        0x13  // prelievo effettuato
#define ID_EVENT_PASSARE_BADGE            0x14  // passare badge
#define ID_EVENT_PASSARE_BADGE_RESP       0x15  // passare badge responsabile
#define ID_EVENT_SELEZIONARE_PRODOTTO     0x16  // selezionare prodotto
#define ID_EVENT_TIMEOUT_SELEZ            0x17  // timeout selezione prodotto
#define ID_EVENT_CELLA_VUOTA              0x18  // cella vuota
#define ID_EVENT_TIMEOUT_BADGE            0x19  // timeout passaggio badge
#define ID_EVENT_SETTORE_VUOTO            0x1A  // settore vuoto
#define ID_EVENT_RICERCA_ZERO             0x1B  // ricerca posizione di zero

#define ID_EVENT_DOOR_OPEN                0x1C  // apertura porta
#define ID_EVENT_DOOR_CLOSE               0x1D  // chiusura porta
#define ID_EVENT_SELECT_PLANE             0x1E  // selezionato piano
#define ID_EVENT_INFO                     0x1F  // richiesto info
#define ID_EVENT_RICARICA_FULL            0x20  // ricarica completa
#define ID_EVENT_RICARICA_PARZ            0x21  // ricarica parziale
#define ID_EVENT_BTN7                     0x22  // selezionato bottone 7
#define ID_EVENT_BTN10_ZERO               0x23  // inizio ricerca zero su bottone 10
#define ID_EVENT_BTN10                    0x24  // selezionato bottone 10
#define ID_EVENT_REFILL                   0x25
#define ID_EVENT_SVUOTA                   0x26
#define ID_EVENT_RICHIUDI                 0x27
#define ID_EVENT_HW_FAILURE               0x28  // guasto HW all'accensione
#define ID_EVENT_BLOCCO_MOTORE            0x29  // blocco motore
#define ID_EVENT_EFFRAZIONE               0x2A  // tentativo effrazione
#define ID_EVENT_SPORTELLO_APERTO         0x2B  // sporello aperto
#define ID_EVENT_END_PROC                 0x40  // fine procedura
