// c2_connect.c
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/net.h>
#include <net/sock.h>
#include <linux/in.h>
#include <linux/inet.h>

static char *c2_ip = "127.0.0.1";
static int port = 9090;
module_param(c2_ip, charp, 0444);
module_param(port, int, 0444);

static struct socket *sock;

static int create_socket(void)
{
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = in_aton(c2_ip),
    };
    int err = 0;

    /* Create Kernel socket */
    err = sock_create_kern(&init_net, PF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
    if (err) {
        sock = NULL;
        return err;
    }

    sock_set_reuseaddr(sock->sk);

    /* Connect to c2 server */
    err = kernel_connect(sock, (struct sockaddr *)&addr, sizeof(addr), 0);
    if (err) {
        sock_release(sock);
        sock = NULL;
        return err;
    }

    pr_info("connected to %s:%d\n", c2_ip, port);
    return 0;
}

static int __init c2_connect_init(void)
{
    int ret;

    pr_info("server connection loading...\n");
    ret = create_socket();
    if (ret) {
        pr_err("socket setup failed: %pe\n", ERR_PTR(ret));
        return ret;
    }
    return 0;
}

static void __exit c2_connect_exit(void)
{
    if (sock) {
        kernel_sock_shutdown(sock, SHUT_RDWR);
        sock_release(sock);
    }
    pr_info("module unloaded\n");
}

module_init(c2_connect_init);
module_exit(c2_connect_exit);
MODULE_LICENSE("GPL");