#include <jni.h>
#include <string>
#include <sys/inotify.h>

#include <fcntl.h>

#include <errno.h>

//android输出日志使用
#include <android/log.h>
#define LOG_TAG "file"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C"
JNIEXPORT void JNICALL
Java_com_example_huozhenpeng_fileobserver_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject job,jstring pathStr,jint sdk) {

    pid_t pid=fork();
    if(pid==0)
    {
        LOGE("fork子进程成功");

        int fd=inotify_init();

        const char *path=env->GetStringUTFChars(pathStr,NULL);
        int wd = inotify_add_watch(fd, path, IN_DELETE);                  //监控指定文件的ALL_EVENTS。
        char event_buf[512];
        struct inotify_event* event;

        while (1)
        {
            int event_pos = 0;
            int num_bytes = read(fd, event_buf, sizeof(event_buf));
            LOGE("文件正在更改");
            if (num_bytes < (int)sizeof(*event))
            {
                if (errno == EINTR)
                    continue;

                LOGE("***** ERROR! android_os_fileobserver_observe() got a short event!");
                return;
            }

            while (num_bytes >= (int)sizeof(*event))
            {
                int event_size;
                event = (struct inotify_event *)(event_buf + event_pos);

                jstring path = NULL;

                if (event->len > 0)
                {
                    path = env->NewStringUTF(event->name);
                }
                if (sdk < 17) {
                    execlp("am", "am", "start", "-a", "android.intent.action.VIEW", "-d",
                           "http://www.baidu.com",NULL);

                } else{
                    execlp("am", "am", "start","--user","0","-a", "android.intent.action.VIEW", "-d",
                           "http://www.baidu.com",NULL);
                }
                LOGE("文件有更改");
//                这个是回调到java层的，利用java代码构造FileObserver的时候不是覆写了个方法onEvent吗
//                env->CallVoidMethod(object, method_onEvent, event->wd, event->mask, path);
                if (env->ExceptionCheck()) {
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                if (path != NULL)
                {
                    env->DeleteLocalRef(path);
                }

                event_size = sizeof(*event) + event->len;
                num_bytes -= event_size;
                event_pos += event_size;
            }
        }




    }
    std::string hello = "Hello from C++";
    return ;
}
