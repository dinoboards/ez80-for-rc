
extern unsigned long cpu_freq_calculated;

void assign_cpu_frequency(const unsigned de) {
  cpu_freq_calculated = ((long)de * 721L) / 100L * 100L;

  if (de >= 10188 && de <= 10229)
    cpu_freq_calculated = 7372800;

  else if (de >= 20350	&& de <= 20464)
    cpu_freq_calculated = 14745600;

  else if (de >= 22123	&& de <= 22205)
    cpu_freq_calculated = 16000000;

  else if (de >= 25442	&& de <= 25586)
    cpu_freq_calculated = 18432000;

  else if (de >= 33069	&& de <= 33314)
    cpu_freq_calculated = 24000000;

  else if (de >= 34511	&& de <= 34702)
    cpu_freq_calculated = 25000000;

  else if (de >= 44178	&& de <= 44423)
    cpu_freq_calculated = 32000000;
}
