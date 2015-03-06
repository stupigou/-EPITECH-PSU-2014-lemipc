/*
** main.c for main in /home/tran_0/rendu/PSU_2014_lemipc
**
** Made by David Tran
** Login   <tran_0@epitech.net>
**
** Started on  Sun Mar  1 15:08:16 2015 David Tran
** Last update Fri Mar  6 20:56:49 2015 David Tran
*/

#include "lemipc.h"

void		print_map(t_princ *lemip)
{
  //  char		*tmp;
  //int		i;

  //  tmp = (char *)lemip->addrmap;
  while (42)
    {
      /*      i = 0;
      while (i < MAP_LEN * MAP_LEN)
	{
	  printf("%d", tmp[i]);
	  if ((i + 1) % MAP_LEN == 0)
	    printf("\n");
	  i++;
	  }*/
      sleep(5);
      //      system("/bin/clear");
    }
}

int		init_memory(t_princ *lemip)
{
  int		ret;

  ret = EXIT_SUCCESS;
  lemip->sops.sem_num = 0;
  lemip->sops.sem_flg = 0;
  if ((lemip->shm_id = shmget(lemip->key,
			      MAP_LEN * MAP_LEN, SHM_R | SHM_W)) == -1)
    {
      if ((lemip->shm_id = shmget(lemip->key, MAP_LEN * MAP_LEN,
				  IPC_CREAT | SHM_R | SHM_W)) == -1)
	return (EXIT_FAILURE);
      if ((lemip->addrmap = shmat(lemip->shm_id, NULL,
				  SHM_R | SHM_W)) == (void *)-1)
	return (EXIT_FAILURE);
      bzero(lemip->addrmap, MAP_LEN * MAP_LEN);
      ret = 2;
    }
  else
    lemip->addrmap = shmat(lemip->shm_id, NULL, SHM_R | SHM_W);
  return (ret);
}

int		init_resources(t_princ *lemip)
{
  char		path[256];
  int		ret;

  srand(time(NULL));
  if ((lemip->key = ftok(getcwd(path, 256), 0)) == -1)
    return (EXIT_FAILURE);
  ret = init_memory(lemip);
  if ((lemip->msg_id = msgget(lemip->key, SHM_R | SHM_W)) == -1)
    lemip->msg_id = msgget(lemip->key, IPC_CREAT | SHM_R | SHM_W);
  if ((lemip->sem_id = semget(lemip->key, 1, SHM_R | SHM_W)) == -1)
    {
      lemip->sem_id = semget(lemip->key, 1, IPC_CREAT | SHM_R | SHM_W);
      semctl(lemip->sem_id, 0, SETVAL, 1);
    }
  return (ret);
}

int		main(int argc, char **argv)
{
  t_princ	lemip;
  int		ret;
  pthread_t	take_map;

  if (argc != 2)
    {
      printf("Usage: %s [number team]\n", argv[0]);
      return (EXIT_SUCCESS);
    }
  if ((ret = init_resources(&lemip)) == -1)
    {
      fprintf(stderr, "Can't create resources requiered for the battle !\n");
      return (EXIT_FAILURE);
    }
  init_player(&lemip, argv[1]);
  if (ret == 2)
    {
      if (pthread_create(&take_map, NULL, ia_thread, &lemip) != 0)
	return (EXIT_FAILURE);
      pthread_join(take_map, NULL);
      print_map(&lemip);
      destroy_resources(&lemip);
    }
  else
    ia_easy(&lemip);
  return (EXIT_SUCCESS);
}
