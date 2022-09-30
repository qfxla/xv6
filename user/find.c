#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "../user/user.h"
#include "../kernel/fs.h"

char *
fmtname(char *path) {
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), 0, DIRSIZ - strlen(p));
    return buf;
}
void
find(char *path, char *target) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if (strcmp(fmtname(path), target) == 0) {
        printf("%s\n", path);
    }

    switch (st.type) {
        case T_FILE:
            break;

        case T_DIR:
            if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("ls: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf); //p为一个指针，指向buf(path)的末尾
            *p++ = '/';
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {
                /**
                 *  因为每个目录的子目录其实都有. 和 .. ，这样做是为了防止死循环
                 */
                if (de.inum == 0 || (strcmp(de.name, ".") == 0 || (strcmp(de.name, "..") == 0)))
                    continue;
                memmove(p, de.name, DIRSIZ);  // 把de.name拼接在p后面
                p[DIRSIZ] = 0;  // p之前是在 '/' 处，现在[DIRSIZ] 表示向后移了DIRSIZ的位置
                find(buf, target);
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("错误\n");
        exit(0);
    }

    if (argc == 2) {
        find(".", argv[2]);
        exit(0);
    }

    find(argv[1], argv[2]);
    exit(0);
}
