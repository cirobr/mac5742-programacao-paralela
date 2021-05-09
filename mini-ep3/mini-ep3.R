library(readxl)
read_xlsx("./rda/execucoes.xlsx", n_max = 10)
dat <- read_xlsx("./rda/execucoes.xlsx", skip = 4)
head(dat)

options(digits = 3)
# agrupar por processo (complilar, executar) e calcular estatística
dat %>%
  group_by(processo) %>%
  summarize(average = mean(tempo_s), standard_deviation = sd(tempo_s))

# agrupar por processo (complilar, executar) + configuração do make, e calcular estatística
dat_out <- dat %>%
  group_by(processo, configuracao) %>%
  summarize(media = mean(tempo_s), desvio_padrao = sd(tempo_s), variancia = desvio_padrao ^ 2)
dat_out

# exportar em formato csv
write_csv(dat_out, "./rda/estatistica.csv")