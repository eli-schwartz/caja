#include <sys/time.h>

#include "../eel/eel-gdk-pixbuf-extensions.h"

#include "test.h"

#define N_SCALES 100

#define DEST_WIDTH 32
#define DEST_HEIGHT 32

int
main (int argc, char* argv[])
{
	GdkPixbuf *pixbuf, *scaled;
	GError *error;
	gint64 t1, t2;
	int i;

	test_init (&argc, &argv);

	if (argc != 2) {
		printf ("Usage: test <image filename>\n");
		exit (1);
	}

	error = NULL;
	pixbuf = gdk_pixbuf_new_from_file (argv[1], &error);

	if (pixbuf == NULL) {
		printf ("error loading pixbuf: %s\n", error->message);
		exit (1);
	}

	printf ("scale factors: %f, %f\n",
		(double)gdk_pixbuf_get_width(pixbuf)/DEST_WIDTH,
		(double)gdk_pixbuf_get_height(pixbuf)/DEST_HEIGHT);

	t1 = g_get_monotonic_time ();
	for (i = 0; i < N_SCALES; i++) {
		scaled = eel_gdk_pixbuf_scale_down (pixbuf, DEST_WIDTH, DEST_HEIGHT);
		g_object_unref (scaled);
	}
	t2 = g_get_monotonic_time ();
	g_print ("Time for eel_gdk_pixbuf_scale_down: %" G_GINT64_FORMAT " msecs\n",
		 (t2 - t1) / G_TIME_SPAN_MILLISECOND);



	t1 = g_get_monotonic_time ();
	for (i = 0; i < N_SCALES; i++) {
		scaled = gdk_pixbuf_scale_simple (pixbuf, DEST_WIDTH, DEST_HEIGHT, GDK_INTERP_NEAREST);
		g_object_unref (scaled);
	}
	t2 = g_get_monotonic_time ();
	g_print ("Time for INTERP_NEAREST: %" G_GINT64_FORMAT " msecs\n",
		 (t2 - t1) / G_TIME_SPAN_MILLISECOND);


	t1 = g_get_monotonic_time ();
	for (i = 0; i < N_SCALES; i++) {
		scaled = gdk_pixbuf_scale_simple (pixbuf, DEST_WIDTH, DEST_HEIGHT, GDK_INTERP_BILINEAR);
		g_object_unref (scaled);
	}
	t2 = g_get_monotonic_time ();
	g_print ("Time for INTERP_BILINEAR: %" G_GINT64_FORMAT " msecs\n",
		 (t2 - t1) / G_TIME_SPAN_MILLISECOND);

	scaled = eel_gdk_pixbuf_scale_down (pixbuf, DEST_WIDTH, DEST_HEIGHT);
	gdk_pixbuf_save (scaled, "eel_scaled.png", "png", NULL, NULL);
	g_object_unref (scaled);

	scaled = gdk_pixbuf_scale_simple (pixbuf, DEST_WIDTH, DEST_HEIGHT, GDK_INTERP_NEAREST);
	gdk_pixbuf_save (scaled, "nearest_scaled.png", "png", NULL, NULL);
	g_object_unref (scaled);

	scaled = gdk_pixbuf_scale_simple (pixbuf, DEST_WIDTH, DEST_HEIGHT, GDK_INTERP_BILINEAR);
	gdk_pixbuf_save (scaled, "bilinear_scaled.png", "png", NULL, NULL);
	g_object_unref (scaled);

	return 0;
}
