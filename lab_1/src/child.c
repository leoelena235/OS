#include "../inc/child.h"

int main(int argc, char **argv)
{
	char buf[4096];
	ssize_t bytes;

	pid_t pid = getpid();

	int32_t file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (file == -1)
	{
		const char msg[] = "error: failed to open requested file\n";
		write(STDERR_FILENO, msg, sizeof(msg));
		exit(EXIT_FAILURE);
	}

	while ((bytes = read(STDIN_FILENO, buf, sizeof(buf))))
	{
		if (bytes < 0)
		{
			const char msg[] = "error: failed to read from stdin\n";
			write(STDERR_FILENO, msg, sizeof(msg));
			exit(EXIT_FAILURE);
		}
		else if (bytes == 1 && buf[0] == '\n')
		{
			const char msg[] = "Empty input detected, exiting client...\n";
			write(STDOUT_FILENO, msg, sizeof(msg) - 1);
			break;
		}

		buf[bytes - 1] = '\0';

		char filtered_buf[256];
		ssize_t filtered_len = 0;

		for (ssize_t i = 0; i < bytes; i++)
		{
			if (!strchr("aeiouAEIOU", buf[i]))
			{
				filtered_buf[filtered_len++] = buf[i];
			}
		}

		int32_t written = write(file, filtered_buf, filtered_len);
		if (written != filtered_len)
		{
			const char msg[] = "error: failed to write to file\n";
			write(STDERR_FILENO, msg, sizeof(msg));
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < filtered_len; i++)
			write(STDOUT_FILENO, &filtered_buf[i], 1);

		write(STDOUT_FILENO, "\n", 1);

		{
			char msg[32];
			const int32_t length = snprintf(msg, sizeof(msg), "Print new word\n");
			write(STDOUT_FILENO, msg, length);
		}
	}

	const char term = '\0';

	ssize_t eof_written = write(file, &term, sizeof(term));
	if (eof_written != sizeof(term))
	{
		const char msg[] = "error: failed to write EOF to file\n";
		write(STDERR_FILENO, msg, sizeof(msg));
		exit(EXIT_FAILURE);
	}

	close(file);
}