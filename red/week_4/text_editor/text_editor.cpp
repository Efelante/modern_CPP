#include <string>
#include <list>
#include <array>
#include <algorithm>
#include "test_runner.h"
using namespace std;

#define MAX_LENGTH 10e6

class Editor {
 public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor();
  void Left();
  void Right();
  void Insert(char token);
  void Cut(size_t tokens = 1);
  void Copy(size_t tokens = 1);
  void Paste();
  string GetText() const;
private:
  list<char> text;
  array<char, 1000000> clipboard;
  list<char>::iterator cursor = text.end();
  size_t cursor_pos = 0;
  size_t clipboard_tokens = 0;
};

Editor::Editor()
{
}

void Editor::Left()
{
	if (cursor_pos != 0){
		cursor = prev(cursor);
		cursor_pos--;
	} else {
		// Cursor is at the beginning
	}
}

void Editor::Right()
{
	if (cursor_pos != text.size()){
		cursor = next(cursor);
		cursor_pos++;
	} else {
		// Cursor is at the end
	}
}

void Editor::Insert(char token)
{
	text.insert(cursor, token);
	cursor_pos++;
}

void Editor::Copy(size_t tokens)
{
	tokens = min(text.size() - cursor_pos, tokens);
	copy_n(cursor, tokens, clipboard.begin());
	clipboard_tokens = tokens;
}

void Editor::Cut(size_t tokens)
{
	tokens = min(text.size() - cursor_pos, tokens);
	copy_n(cursor, tokens, clipboard.begin());
	clipboard_tokens = tokens;

	// Erase data in the text
	for (size_t i = 0; i < tokens; ++i){
		cursor = text.erase(cursor);
	}
}

void Editor::Paste()
{
	for (size_t i = 0; i < clipboard_tokens; ++i){
		Insert(clipboard[i]);
	}
}

string Editor::GetText() const
{
	return string(text.begin(), text.end());
}

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  //{
  //  Editor editor;
  //  
  //  TypeText(editor, "misprnit");
  //  editor.Left();
  //  editor.Left();
  //  editor.Left();
  //  editor.Cut(1);
  //  editor.Right();
  //  editor.Paste();
  //  
  //  ASSERT_EQUAL(editor.GetText(), "misprint");
  //}
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}
