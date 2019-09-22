
#include <stdio.h>  
#include <jni.h> 


jint create_vm(JavaVM** jvm, JNIEnv** env) 
{  
    JavaVMInitArgs args;  
    JavaVMOption options[1];  
    args.version = JNI_VERSION_1_6;  
    args.nOptions = 1;  
    options[0].optionString = "-Djava.class.path=./";  
    args.options = options;  
    args.ignoreUnrecognized = JNI_FALSE;  
    return JNI_CreateJavaVM(jvm, (void **)env, &args);  
}  


int main(int argc, char **argv)
{
	JavaVM* jvm;
	JNIEnv* env;

	jclass cls;
	int ret = 0;

	jmethodID mid;
	jmethodID cid;

	jobject jobj;
	jstring jstr;

	int r;
		
	/* 1. create java virtual machine */
	if (create_vm(&jvm, &env)) {
		printf("can not create jvm\n");
		return -1;
	}

	/* 2. get class */
	cls = (*env)->FindClass(env, "Hello");
	if (cls == NULL) {
		printf("can not find hello class\n");
		ret = -1;
		goto destroy;
	}

	/* 3. create object 
	 * 3.1 get constructor method
	 * 3.2 create parameters
	 * 3.3 NewObject
	 */

	/* Get the method ID for the String constructor */
	cid = (*env)->GetMethodID(env, cls,	"<init>", "()V");
	if (cid == NULL) {
		ret = -1;
		printf("can not get constructor method");
		goto destroy;
	}

	jobj = (*env)->NewObject(env, cls, cid);
	if (jobj == NULL) {
		ret = -1;
		printf("can not create object");
		goto destroy;
	}

	/* 4. call method
	 * 4.1 get method
	 * 4.2 create parameter
	 * 4.3 call method
	 */

	mid = (*env)->GetMethodID(env, cls, "sayhello_to","(Ljava/lang/String;)I");
	if (mid == NULL) {
		ret = -1;
		printf("can not get method\n");
		goto destroy;
	}

	jstr = (*env)->NewStringUTF(env, "weidongshan@qq.com");

	r = (*env)->CallIntMethod(env, jobj, mid, jstr);
	printf("ret = %d\n", r);

destroy:

	(*jvm)->DestroyJavaVM(jvm);
	return ret;
}

