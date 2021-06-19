library(tidyverse)
f <- "experiment_results.csv"
#if(!exists("df")) 
df <- read_csv(f)
head(df)

df$program    <- as.factor(df$program)
df$threads    <- as.factor(df$threads)
df$image      <- as.factor(df$image)
df$image_size <- as.factor(df$image_size)

# qual o programa mais rápido?
df %>% 
  group_by(program) %>%
  summarize(m = mean(time_sec), s = sd(time_sec))

# boxplot por programa
df %>%
  ggplot(aes(x=program, y=time_sec, 
             fill=program)) +
  geom_boxplot(outlier.size=0.7) +
  ggtitle("Média de velocidade por programa") +
  xlab("") +
  scale_y_continuous(trans='log2',
                     labels = scales::number_format(accuracy = 0.001)) +
  theme(axis.text.x = element_text(angle = 45, hjust = 1, size=8),
        axis.text.y = element_text(size=8), 
        legend.position = "none")

# boxplot por programa por imagem
df %>%
  group_by(program) %>%
  ggplot(aes(x=program, y=time_sec, fill=image)) +
  geom_boxplot(outlier.size=0.7) +
  ggtitle("Média de velocidade por programa") +
  xlab("") +
  scale_y_continuous(trans='log2',
                     labels = scales::number_format(accuracy = 0.001)) +
  theme(axis.text.x = element_text(angle = 45, hjust = 1, size=8),
        axis.text.y = element_text(size=8),
        legend.position = "bottom")

# boxplot por programa por thread
df %>%
  group_by(program) %>%
  ggplot(aes(x=program, y=time_sec, fill=threads)) +
  geom_boxplot(outlier.size=0.7) +
  ggtitle("Média de velocidade por programa") +
  xlab("") +
  scale_y_continuous(trans='log2',
                     labels = scales::number_format(accuracy = 0.001)) +
  theme(axis.text.x = element_text(angle = 45, hjust = 1, size=8),
        axis.text.y = element_text(size=8),
        legend.position = "bottom")

# boxplot thread 1
df %>%
  group_by(program) %>%
  filter(threads == 1) %>%
  ggplot(aes(x=program, y=time_sec, fill=threads)) +
  geom_boxplot(outlier.size=0.7) +
  ggtitle("Média de velocidade Thread 1") +
  xlab("") +
  scale_y_continuous(trans='log2',
                     labels = scales::number_format(accuracy = 0.001)) +
  theme(axis.text.x = element_text(angle = 45, hjust = 1, size=8),
        axis.text.y = element_text(size=8),
        legend.position = "none")

# boxplot all threads
df %>%
  group_by(program) %>%
  filter(threads %in% c(2, 4, 6, 8, 16, 32)) %>%
  ggplot(aes(x=program, y=time_sec, fill=threads)) +
  geom_boxplot(outlier.size=0.7) +
  ggtitle("Média de velocidade por Thread (2 a 32)") +
  xlab("") +
  scale_y_continuous(trans='log2',
                     labels = scales::number_format(accuracy = 0.001)) +
  theme(axis.text.x = element_text(angle = 45, hjust = 1, size=8),
        axis.text.y = element_text(size=8))

# grid all threads
df %>%
  filter(threads %in% c(2, 4, 6, 8, 16, 32)) %>%
  ggplot(aes(x=program, y=time_sec, fill=threads)) +
  geom_boxplot(outlier.size=0.7) +
  facet_grid(. ~ threads) +
  ggtitle("Comparativo de velocidade por Thread (2 a 32)") +
  xlab("") +
  scale_y_continuous(trans='log2',
                     labels = scales::number_format(accuracy = 0.001)) +
  theme(axis.text.x = element_text(angle = 45, hjust = 1, size=8),
        axis.text.y = element_text(size=8),
        legend.position = "none")

# grid all threads all figures
df %>%
  filter(threads %in% c(2, 4, 6, 8, 16, 32)) %>%
  ggplot(aes(x=program, y=time_sec, fill=threads)) +
  geom_boxplot(outlier.size=0.7) +
  facet_grid(image ~ threads) +
  ggtitle("Comparativo de velocidade por Thread e Imagem") +
  xlab("") +
  scale_y_continuous(trans='log2',
                     labels = scales::number_format(accuracy = 0.001)) +
  theme(axis.text.x = element_text(angle = 45, hjust = 1, size=8),
        axis.text.y = element_text(size=8),
        legend.position = "none")
