#include "gntbutton.h"
#include "gnt.h"
#include "gntkeys.h"
#include "gnttree.h"
#include "gntbox.h"
#include "gntentry.h"
#include "gntlabel.h"

static void
toggled(GntWidget *tree, gpointer key, gpointer null)
{
	GntWidget *w = gnt_box_new(FALSE, FALSE);

	gnt_box_set_toplevel(GNT_BOX(w), TRUE);

	gnt_box_add_widget(GNT_BOX(w),
			gnt_label_new(gnt_tree_get_choice(GNT_TREE(tree), key) ? "Selected" : "NOT"));
	gnt_widget_show(w);
}

int main()
{
	freopen(".error", "w", stderr);
	gnt_init();

	GntWidget *label = gnt_label_new("So wassup dudes and dudettes!!\nSo this is, like,\nthe third line!! \\o/");
	GntWidget *vbox, *hbox, *tree, *box, *button;
	WINDOW *test;

	box(stdscr, 0, 0);
	wrefresh(stdscr);

	vbox = gnt_box_new(FALSE, FALSE);
	hbox = gnt_box_new(FALSE, TRUE);

	gnt_widget_set_name(vbox, "vbox");
	gnt_widget_set_name(hbox, "hbox");

	gnt_box_add_widget(GNT_BOX(hbox), label);

	GntWidget *entry = gnt_entry_new("a");
	gnt_widget_set_name(entry, "entry");
	gnt_box_add_widget(GNT_BOX(hbox), entry);

	box = gnt_box_new(FALSE, FALSE);
	tree = gnt_tree_new();
	gnt_widget_set_name(tree, "tree");
	gnt_box_add_widget(GNT_BOX(box), tree);
	gnt_box_add_widget(GNT_BOX(hbox), box);

	gnt_tree_add_row_after(GNT_TREE(tree), "a", "a", NULL, NULL);
	gnt_tree_add_row_after(GNT_TREE(tree), "c", "c", NULL, NULL);
	gnt_tree_add_row_after(GNT_TREE(tree), "d", "d", NULL, NULL);
	gnt_tree_add_row_after(GNT_TREE(tree), "e", "e", "a", NULL);
	gnt_tree_add_choice(GNT_TREE(tree), "b", "b", "d", NULL);

	GNT_WIDGET_UNSET_FLAGS(hbox, GNT_WIDGET_NO_BORDER | GNT_WIDGET_NO_SHADOW);
	gnt_box_set_title(GNT_BOX(hbox), "This is the title …");


	g_signal_connect(G_OBJECT(tree), "toggled", G_CALLBACK(toggled), NULL);

	button = gnt_button_new("one");
	gnt_widget_set_name(button, "one");
	gnt_box_add_widget(GNT_BOX(vbox), button);

	button = gnt_button_new("two");
	gnt_widget_set_name(button, "two");
	gnt_box_add_widget(GNT_BOX(vbox), button);

	button = gnt_button_new("three");
	gnt_widget_set_name(button, "three");
	gnt_box_add_widget(GNT_BOX(vbox), button);

	gnt_box_add_widget(GNT_BOX(hbox), vbox);
	
	gnt_widget_show(hbox);

	gnt_main();

	gnt_quit();

	return 0;
}

