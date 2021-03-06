/*
** ia_easy.c for ia_easy in /home/tran_0/rendu/PSU_2014_lemipc
** 
** Made by David Tran
** Login   <tran_0@epitech.net>
** 
** Started on  Thu Mar  5 18:27:11 2015 David Tran
** Last update Sun Mar  8 20:52:33 2015 David Tran
*/

#include "lemipc.h"

int	is_alive(t_princ *lemip)
{
  char	*tmp;
  t_pos	square;
  char	ok;

  ok = 0;
  tmp = (char *)lemip->addrmap;
  square.y = lemip->player.ia.y - 1;
  while (square.y <= lemip->player.ia.y + 1)
    {
      square.x = lemip->player.ia.x - 1;
      while (square.x <= lemip->player.ia.x + 1)
	{
	  if (square.x >= 0 && square.x < MAP_LEN &&
	      square.y >= 0 && square.y < MAP_LEN &&
	      tmp[square.y * MAP_LEN + square.x] != 0 &&
	      tmp[square.y * MAP_LEN + square.x] != lemip->player.team)
	    ok++;
	  square.x++;
	}
      square.y++;
    }
  return (ok);
}

void	move_ia(t_princ *lemip)
{
  char	xmove;
  char	ymove;
  char	*tmp;

  srand(time(NULL));
  tmp = (char *)lemip->addrmap;
  xmove = rand() % 3 - 1;
  ymove = rand() % 3 - 1;
  if ((lemip->player.ia.x + xmove) >= 0 && (lemip->player.ia.y + ymove) >= 0 &&
      (lemip->player.ia.x + xmove) < MAP_LEN &&
      (lemip->player.ia.y + ymove) < MAP_LEN &&
      tmp[(lemip->player.ia.y + ymove) * MAP_LEN + lemip->player.ia.x + xmove]
      == 0)
    {
      tmp[lemip->player.ia.y * MAP_LEN + lemip->player.ia.x] = 0;
      lemip->player.ia.y += ymove;
      lemip->player.ia.x += xmove;
      tmp[lemip->player.ia.y * MAP_LEN + lemip->player.ia.x]
	= lemip->player.team;
    }
}

int	ia_easy(t_princ *lemip)
{
  char	*tmp;

  tmp = (char *)lemip->addrmap;
  while (42)
    {
      if (tmp[MAP_LEN * MAP_LEN + 1] == -1)
	return (EXIT_SUCCESS);
      lemip->sops.sem_op = -1;
      if (semop(lemip->sem_id, &lemip->sops, 1) == -1)
	return (EXIT_FAILURE);
      sleep(1);
      lemip->sops.sem_op = 1;
      if (is_alive(lemip) > 1)
	{
	  tmp[lemip->player.ia.y * MAP_LEN + lemip->player.ia.x] = 0;
	  if (semop(lemip->sem_id, &lemip->sops, 1) == -1)
	    return (-1);
	  return (2);
	}
      move_ia(lemip);
      if (semop(lemip->sem_id, &lemip->sops, 1) == -1)
	return (-1);
    }
}

int	ia_thread(t_princ *lemip)
{
  lemip->sops.sem_op = -1;
  if (semop(lemip->sem_id, &lemip->sops, 1) == -1)
    return (EXIT_FAILURE);
  lemip->sops.sem_op = 1;
  if (is_alive(lemip) > 1)
    {
      if (semop(lemip->sem_id, &lemip->sops, 1) == -1)
	return (-1);
      return (-1);
    }
  move_ia(lemip);
  if (semop(lemip->sem_id, &lemip->sops, 1) == -1)
    return (-1);
  return (0);
}

int	launch_ia_easy(t_princ *lemip, char *tmp)
{
  if (lemip->exit == 0)
    if (ia_thread(lemip) == -1)
      {
	lemip->exit = 1;
	tmp[lemip->player.ia.y * MAP_LEN + lemip->player.ia.x] = 0;
      }
  return (0);
}
