#include <iostream>
#include <string>

class ProgressBar {
private:
	char _progress;
	char _nodes_count;

	std::string _label;

public:

	ProgressBar(char nodes_count, std::string label = "Progress") :
		_progress(-1),
		_nodes_count(nodes_count),

		_label(label)
	{
	}

	void progress_to(char progress, bool do_not_rewrite = false, bool force_draw = false) {
		progress = std::min(progress, _nodes_count);

		if (_progress != progress || force_draw) {
            std::cout << _label << ": |";
			
            for ( int j = 0; j < progress; j++)
                std::cout << (char)219;
            for ( int j = progress; j < _nodes_count; j++)
                std::cout << '-';
				
            std::cout << "| " << (int)((progress * 100) /_nodes_count) << "\% ";

			if ((progress != _nodes_count) && (!do_not_rewrite))
				std::cout << "\r";

			_progress = progress;
        }
	}

	void set_progress_nodraw(char progress) {
		_progress = progress;
	}

	bool is_change(char progress) {
		return progress != _progress;
	}

};

