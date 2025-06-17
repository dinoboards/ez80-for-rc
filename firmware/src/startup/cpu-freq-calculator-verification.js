cpu_freq_calculated = 32000000

function bus_cycle_to_ns(bc, cpu_mhz) { return (bc * 3000) / cpu_mhz; }

function ws_to_ns(ws, cpu_mhz) { return ((ws + 1) * 1000) / cpu_mhz; }

// Min state
// minimum_states - top most bit, if set, must return as Bus Cycle
function calculate_wait_state(min_nanoseconds, minimum_states /* only 8 bit value accepted */) {
  let cpu_mhz = cpu_freq_calculated / 1000000;
  let must_be_BC = minimum_states & 0x80;
  let state;

  if (must_be_BC) {
    for (state = minimum_states & 0x7F; state <= 7; state++) {
      if (bus_cycle_to_ns(state, cpu_mhz) >= min_nanoseconds)
        return state + 0x80;
    }

    return 0x87;
  }

  for (state = 0; state <= 15; state++)
    if (ws_to_ns(state, cpu_mhz) >= min_nanoseconds)
      return state;

  return 15;
}

function main() {
  let cpu_mhz = cpu_freq_calculated / 1000000;

  console.log(`CPU ${cpu_freq_calculated/1000000}Mhz`)
  for(bc = 1; bc <=7; bc++ ) {
    console.log(`B/C ${bc}: ${bus_cycle_to_ns(bc, cpu_mhz)}ns`)
  }

  for(ws = 0; ws <=15; ws++ ) {
    console.log(`W/S ${ws}: ${ws_to_ns(ws, cpu_mhz)}ns`)
  }

  console.log("")

  console.log(`130,0x80 = ${calculate_wait_state(130, 0x80)-0x80}`)
  console.log(`60,0x80 = ${calculate_wait_state(60, 0x00)}`)
}


main()
