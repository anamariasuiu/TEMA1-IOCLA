#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir {
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;

void touch (Dir* parent, char* name) 
{
	int gasit = 0;
	
	if(parent->head_children_files) 
	{
		File* head_children = parent->head_children_files;
		while(head_children != NULL)
		{
			if(strcmp(name, head_children->name) == 0)
			{
				printf("File already exists\n");
				gasit = 1;
				break;
			} else head_children = head_children->next;

		}
	}

    if(parent->head_children_dirs) 
	{
		Dir* head_children = parent->head_children_dirs;
		while(head_children)
		{
			if(strcmp(name, head_children->name) == 0)
			{
				printf("File already exists\n");
				gasit = 1;
				break;
			} else head_children= head_children->next;

		}
	}

	if(gasit == 0) 
	{
		struct File* new_file = (struct File*)malloc(sizeof(struct File));
		new_file->name = (char*)malloc((strlen(name)+1) *sizeof(char));
		strcpy(new_file->name, name);
		new_file->next = NULL;
		new_file->parent = parent;

		if(parent->head_children_files!= NULL) 
		{
	        File *files = parent->head_children_files;
		    while(files->next)
			{
				files = files->next;
			}
			files->next = new_file;
		}
		else 
		{
			parent->head_children_files = new_file;
		}
	}
}

void mkdir (Dir* parent, char* name)
{
	int gasit = 0;
	if(parent->head_children_files) 
	{
		File *head = parent->head_children_files;
		while(head)
		{
			if(strcmp(name, head->name) == 0)
			{
				printf("Directory already exists\n");
				gasit = 1;
				break;
			} else head = head->next;

		}
	}
	if(parent->head_children_dirs) 
	{
		Dir *head = parent->head_children_dirs;
		while(head)
		{
			if(strcmp(name, head->name) == 0)
			{
				printf("Directory already exists\n");
				gasit = 1;
				break;
			} else head = head->next;

		}
	}
	if(gasit == 0) 
	{
		Dir* new_dir = (Dir*)calloc(1,sizeof(Dir));
		new_dir->name = (char*)malloc(strlen(name)+1 *sizeof(char));
		strcpy(new_dir->name, name);
		new_dir->parent = parent;
		new_dir->next = NULL;
		new_dir->head_children_files = NULL;
		new_dir->head_children_dirs = NULL;
		Dir *dirs = parent->head_children_dirs;

		if(dirs == NULL) 
		{
			parent->head_children_dirs = new_dir;
		}
		else 
		{
			while(dirs->next) 
			{
				dirs = dirs->next;
			}
			dirs->next = new_dir;
		}
	}
}

void ls (Dir* parent) 
{
	if(parent->head_children_dirs) 
	{
		Dir *dir = parent->head_children_dirs;
        while(dir)
		{
			printf("%s\n", dir->name);
			dir = dir->next;
		}
	}
	if(parent->head_children_files) 
	{
		File *file = parent->head_children_files;
		while(file) 
		{
			printf("%s\n",file->name);
			file = file->next;
		}
	}
}

void rm (Dir* parent, char* name) 
{
	int gasit = 0;
	File *file = parent->head_children_files;

	if(file != NULL) {
		if(strcmp(name, parent->head_children_files->name) == 0) 
		{
			parent->head_children_files = parent->head_children_files->next;
			free(file);
			gasit = 1;
		}
		else 
		{
			File *prev_file = file;
			file = file->next;

			while(file != NULL)
			{
				if(strcmp(name, file->name) == 0) 
				{
					gasit = 1;
					prev_file->next = file->next;
					free(file);
					break;
				} 
				else 
				{
					file = file->next;
					prev_file = prev_file->next;
				}

			}
		}
	}
	if(gasit == 0)
	printf("Could not find the file\n");
}

void rmdir (Dir* parent, char* name) 
{
	int gasit = 0;
	Dir* dir = parent->head_children_dirs;

	if(dir != NULL) 
	{
		if(strcmp(name, parent->head_children_dirs->name) == 0) 
		{
			parent->head_children_dirs = parent->head_children_dirs->next;
			free(dir);
			gasit = 1;
		}
		else 
		{
			Dir *prev_dir = dir;
			dir = dir->next;

			while(dir != NULL)
			{
				if(strcmp(name, dir->name) == 0) 
				{
					gasit = 1;
					prev_dir->next = dir->next;
					free(dir);
					break;
				} 
				else 
				{
					dir = dir->next;
					prev_dir = prev_dir->next;
				}

			}
		}

	}
	if(gasit == 0) 
	{
		printf("Could not find the dir\n");
	}
}

void cd(Dir** target, char *name) 
{
	if(strcmp(name,"..") == 0)
	{
		if((*target)->parent != NULL) 
		{
			*target = (*target)->parent;
		}
	} 
	else 
	{
		int gasit = 0;
		Dir *dirs = (*target)->head_children_dirs;
		while(dirs != NULL) 
		{
			if(strcmp(name, dirs->name) == 0) 
			{
				gasit = 1;
				*target = dirs;
				break;
			}
			else dirs = dirs->next;
		}
		if(gasit == 0) 
		{
			printf("No directories found!\n");
		}
	}

}

char *pwd (Dir* target) 
{
	char *cale1 = (char*)malloc(100*sizeof(char));
	strcpy(cale1, "");
	char *cale2 = (char*)malloc(100*sizeof(char));
	strcpy(cale2, "");
	int contor = 0;

	Dir *dirs = target;
	strcat(cale1, "/");
	strcat(cale1, dirs->name);
	dirs = dirs->parent;

	while(dirs) 
	{
		contor++;
        if(contor % 2 == 1) 
		{
			strcat(cale2, "/");
			strcat(cale2, dirs->name);
			strcat(cale2,cale1);
			strcpy(cale1 , "");
		}

		if(contor % 2 == 0) 
		{
			strcat(cale1, "/");
			strcat(cale1, dirs->name);
			strcat(cale1,cale2);
			strcpy(cale2 , "");
		}
		dirs = dirs->parent;
	}
	
	if(contor % 2 == 0)
    return cale1;
	else return cale2;
}

void stop (Dir* target) 
{
	if(target) 
	{
		stop(target->head_children_dirs);
	    Dir *aux = target;

		while(aux->head_children_files) 
		{
			File *f = aux->head_children_files;
			aux->head_children_files = aux->head_children_files->next;
			free(f->name);
			free(f);
		}

		if(target->next) 
		{
			stop(target->next);
		}

		free(target->name);
		free(target);
	}
}

void tree (Dir* target, int level) 
{
	Dir *parent = target;
	if(parent->head_children_dirs) 
	{
		for(int i = 0; i < level; i++) 
		{
			printf("    ");
		}
		printf("%s\n", parent->head_children_dirs->name);
		tree(parent->head_children_dirs, level + 1);

		if(parent->head_children_files) 
		{
			File *file = parent->head_children_files;
			while(file)
			{
				for(int i = 0; i < level; i++) 
				{	
					printf("    ");
				}
				printf("%s\n", file->name);
				file = file->next;
			}
		}
		if(parent->head_children_dirs->next) 
		{
			for(int i = 0; i < level; i++)
			{
				printf("    ");		
			}
			printf("%s\n", parent->head_children_dirs->next->name);
		    tree(parent->head_children_dirs->next, level);
		}
	}
}

void mv(Dir* parent, char *oldname, char *newname)
{
	int gasit_dir = 0, gasit_file = 0, gasit_new = 0;
	Dir *dirs = parent->head_children_dirs;
	Dir *copie_dir;
	File *files = parent->head_children_files;
	File *copie_file;
	while(dirs)
	{
		if(strcmp(dirs->name, oldname) == 0)
		{
			gasit_dir = 1;
			break;
		}
		else dirs = dirs->next;
	}
	while(files)
	{
		if(strcmp(files->name, oldname) == 0)
		{
			gasit_file = 1;
			break;
		}
		else files = files->next;
	}
	if(gasit_dir == 0 && gasit_file == 0)
	{
		printf("File/Director not found\n");
	}
	else 
	{
		Dir *dir = parent->head_children_dirs;
		File *file = parent->head_children_files;
		while(dir)
		{
			if(strcmp(dir->name, newname) == 0)
			{
				gasit_new = 1;
				break;
			}
			else dir = dir->next;
		}
		while(file)
		{
			if(strcmp(file->name, newname) == 0)
			{
				gasit_new = 1;
				break;
			}
			else file = file->next;
		}
		if(gasit_new == 1)
		{
			printf("File/Director already exists\n");
		}
		else
		{
			Dir *dir = parent->head_children_dirs;
			if(gasit_dir == 1) {
				if(parent->head_children_dirs->next){
					while(dir->next)
					{
						if(strcmp(dir->name, oldname) == 0)
						{
							copie_dir = dir;
							copie_dir->head_children_dirs = dir->head_children_dirs;
							copie_dir->head_children_files = dir->head_children_files;
							strcpy(copie_dir->name, newname);
							Dir *dirut = dir;
							rmdir(parent, dirut->name);
						}
						dir = dir->next;
					}
					if(strcmp(dir->name, oldname) == 0)
					{
						copie_dir = dir;
						copie_dir->head_children_dirs = dir->head_children_dirs;
						copie_dir->head_children_files = dir->head_children_files;
						strcpy(copie_dir->name, newname);
						rmdir(parent, dir->name);
					}
					dir->next = copie_dir;
					copie_dir->next = NULL;
				}
				else {
					if(strcmp(dir->name, oldname) == 0) {
	
						strcpy(dir->name, newname);
					}

				}
			}
			File *file = parent->head_children_files;
			if(gasit_file == 1) {
				int ok = 0;
				if(parent->head_children_files->next) {
					while(file->next)
					{
						ok++;
						if(strcmp(file->name, oldname) == 0)
						{
							copie_file = file;
							strcpy(copie_file->name, newname);
							rm(parent, file->name);
						}
						file = file->next;
					}
						if(strcmp(file->name, oldname) == 0)
						{
							copie_file = file;
							strcpy(copie_file->name, newname);
							rm(parent, file->name);

						}
						file->next = copie_file;
						copie_file->next = NULL;
				}	
				else {
					if(strcmp(file->name, oldname) == 0) {
	
						strcpy(file->name, newname);
					}

				}
			} 
		}

	}

}

int main () {
	Dir* root = (Dir*)calloc(1,sizeof(Dir));
	root->name = (char*)malloc(sizeof(char)*(strlen("home")+1));
	strcpy(root->name, "home");
	root->parent = NULL;
	root->head_children_files = NULL;
	root->head_children_dirs = NULL;
	root->next = NULL;
	char *string = (char*)calloc(100,sizeof(char));
	Dir *parent = root;
	do
	{
		size_t size = 0;
		free(string);
		getline(&string, &size, stdin);

		if(strncmp(string, "touch", 5) == 0) 
		{
			char *name;
			name = strtok(string, " ");
			name = strtok(NULL, "\n");
			touch(parent, name);
		}
		if(strncmp(string, "mkdir", 5) == 0) 
		{
			char *name;
			name = strtok(string, " ");
			name = strtok(NULL, "\n");
			mkdir(parent, name);
		}
		if(strncmp(string, "ls", 2) == 0) 
		{
			ls(parent);
		}
		if(strncmp(string, "rm", 2) == 0 && strncmp(string, "rmdir", 5) != 0)
		{
			char *name;
			name = strtok(string, " ");
			name = strtok(NULL, "\n");
			rm(parent, name);

		}
		if(strncmp(string, "rmdir", 5) == 0)
		{
			char *name;
			name = strtok(string, " ");
			name = strtok(NULL, "\n");

			rmdir(parent, name);

		}
		if(strncmp(string, "cd", 2) == 0) 
		{
			char *name;
			name = strtok(string, " ");
			name = strtok(NULL, "\n");
			cd(&parent, name);
		}
		if(strcmp(string, "pwd\n") == 0)
		{
			printf("%s\n", pwd(parent));
		}
		if(strncmp(string, "mv", 2) == 0)
		{
			char *oldname, *newname;
			oldname = strtok(string, " ");
			oldname = strtok(NULL, " ");
			newname = strtok(NULL, "\n");

			mv(parent, oldname, newname);
		}
		if(strcmp(string, "tree\n") == 0)
		{
			tree(parent, 0);
		}
		if(strncmp(string, "stop", 4) == 0) 
		{
			stop(root);
		}

	} while (strcmp(string,"stop\n") != 0);

	free(string);
	return 0;
}
