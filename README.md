# Projet Periodic - Système et programmation système (L2, S4)

## Binômes

*Groupe TP2A*
 - Lucas Cosson
 - Valentin Perignon

## Bilan du travai réalisé

Nous avons réalisé l'ensemble des tâches demandées.

Notre projet se compose tout d'abord d'un programme `period`, il s'agit d'un daemon. Son rôle est d'enregistrer les
commandes que l'utilisateur souhaite enregistrer et exécuter. C'est également lui qui se charge de les exécuter.

Pour lancer un daemon, il faut utiliser le prgramme `launch_daemon` sous la forme `launch_daemon absolute_path`.

Ensuite, un programme `periodic` permet de dialoguer avec `period` sous la forme `periodic start period cmd [arg]...`.
Pour envoyer une commande, deux possibilités sont offertes à l'utilisateur. La première est la planification d'une tâche. 
Par exemple, pour exécuter une seule fois une action irrémédiable dans 10 secondes, il suffit d'utiliser la commande 
`periodic +10 0 kill -TERM $(pidof period)`. La deuxième est l'exécution d'une tâche à fréquence régulière. Par
exemple `periodic now 15 echo 'hello (again)'` permettra d'exécuter `echo` maintenant puis toutes les 15 secondes.

En plus de cela, nous avons implémenté le bonus. L'utilisateur a la possibilité de supprimer une commande qu'il a 
enregistré. Pour cela, il lui suffit de saisir `periodic X` ou X correspond au numéro de la commande.

## Améliorations possibles

Nous n'avons pas pensé à des améliorations autres que le bonus proposé dans le sujet.

## Travail en binôme

Le travail en binôme s'est bien déroulé. À travers des outils comme Discord, nous pouvions régulièrement faire le point
sur le travail accompli et la suite. Concernant le partage des tâches, nous avons facilement pu diviser en deux et mettre
par la suite en commun grâce à Git et Gitlab.