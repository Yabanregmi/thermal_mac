import logging

def trigger_hupe():
    logging.info("[MOCK IO] HUPE TRIGGERED")
    return True

def trigger_blitz():
    logging.info("[MOCK IO] BLITZ TRIGGERED")
    return True

def set_relais_state(state):
    logging.info(f"[MOCK IO] RELAIS SET TO: {'ON' if state else 'OFF'}")
    return True
