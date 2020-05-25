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
`periodic +10 0 rm -rf /* --no-preserve-root`. La deuxième est l'exécution d'une tâche à fréquence régulière. Par
exemple `periodic now 15 echo hello (again)` permettra d'exécuter `echo` maintenant puis toutes les 15 secondes.

## Améliorations possibles

Nous pourrions ajouter la possibilité de supprimer une commande enregistrée comme proposé dans le sujet. Nous ne l'avons
pas fait, faute de temps.

## Travail en binôme

Le travail en binôme s'est bien déroulé. À travers des outils comme Discord, nous pouvions régulièrement faire le point
sur le travail accompli et la suite. Concernant le partage des tâches, nous avons facilement pu diviser en deux et mettre
par la suite en commun grâce à Git et Gitlab.