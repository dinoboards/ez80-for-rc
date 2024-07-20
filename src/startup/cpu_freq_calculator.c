extern unsigned long cpu_freq_calculated;

#define TIMER_FACTOR (120)

#define UPPER_FOR_32MHZ ((32000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_32MHZ (UPPER_FOR_32MHZ-1024)
#define UPPER_FOR_25MHZ ((25000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_25MHZ (UPPER_FOR_25MHZ-1024)
#define UPPER_FOR_24MHZ ((24000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_24MHZ (UPPER_FOR_24MHZ-1024)
#define UPPER_FOR_20MHZ ((20000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_20MHZ (UPPER_FOR_20MHZ-1024)
#define UPPER_FOR_18MHZ ((18432000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_18MHZ (UPPER_FOR_18MHZ-1024)
#define UPPER_FOR_16MHZ ((16000000 / TIMER_FACTOR) + 64)
#define LOWER_FOR_16MHZ (UPPER_FOR_16MHZ-1024)
#define UPPER_FOR_14MHZ ((14745600 / TIMER_FACTOR) + 64)
#define LOWER_FOR_14MHZ (UPPER_FOR_14MHZ-1024)
#define UPPER_FOR_7MHZ ((7372800 / TIMER_FACTOR) + 64)
#define LOWER_FOR_7MHZ (UPPER_FOR_7MHZ-1024)

void assign_cpu_frequency(const unsigned de) {
  cpu_freq_calculated = ((long)de * (long)TIMER_FACTOR) / 100L * 100L;

  if (de >= LOWER_FOR_7MHZ && de <= UPPER_FOR_7MHZ)
    cpu_freq_calculated = 7372800;

  else if (de >= LOWER_FOR_14MHZ && de <= UPPER_FOR_14MHZ)
    cpu_freq_calculated = 14745600;

  else if (de > LOWER_FOR_16MHZ && de <= UPPER_FOR_16MHZ)
    cpu_freq_calculated = 16000000;

  else if (de > LOWER_FOR_18MHZ && de <= UPPER_FOR_18MHZ)
    cpu_freq_calculated = 18432000;

  else if (de >= LOWER_FOR_20MHZ && de <= UPPER_FOR_20MHZ)
    cpu_freq_calculated = 20000000;

  else if (de >= LOWER_FOR_24MHZ && de <= UPPER_FOR_24MHZ)
    cpu_freq_calculated = 24000000;

  else if (de >= LOWER_FOR_25MHZ && de <= UPPER_FOR_25MHZ)
    cpu_freq_calculated = 25000000;

  else if (de >= LOWER_FOR_32MHZ && de <= UPPER_FOR_32MHZ)
    cpu_freq_calculated = 32000000;
}
