#include <string>
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
  string text;
  string clipboard;
  size_t cursor_pos = 0;
};

Editor::Editor()
{
	text.reserve(MAX_LENGTH);
	clipboard.reserve(MAX_LENGTH);
}

void Editor::Left()
{
	if (cursor_pos){
		cursor_pos -= 1;
	} else {
		// Cursor is at the beginning
	}
}

void Editor::Right()
{
	if (cursor_pos < text.size()){
		cursor_pos += 1;
	} else {
		// Cursor is at the end
	}
}

void Editor::Insert(char token)
{
	text.insert(cursor_pos, 1, token);
	cursor_pos += 1;
}

void Editor::Copy(size_t tokens)
{
	tokens = min(text.size() - cursor_pos, tokens);
	if (0 == tokens){
		clipboard.clear();
	} else {
		clipboard.insert(0, text, 0, tokens);
	}
}

void Editor::Cut(size_t tokens)
{
	tokens = min(text.size() - cursor_pos, tokens);
	if (0 == tokens){
		clipboard.clear();
	} else {
		clipboard.insert(0, text, cursor_pos, tokens);
		text.erase(cursor_pos, tokens);
	}
}

void Editor::Paste()
{
	text.insert(cursor_pos, clipboard);
	cursor_pos += clipboard.size();
}

string Editor::GetText() const
{
	return text;
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
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
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
