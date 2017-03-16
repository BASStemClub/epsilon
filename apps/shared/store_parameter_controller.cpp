#include "store_parameter_controller.h"
#include <assert.h>

namespace Shared {

StoreParameterController::StoreParameterController(Responder * parentResponder, FloatPairStore * store) :
  ViewController(parentResponder),
  m_deleteColumn(MessageTableCell(I18n::Message::ClearColumn)),
  m_copyColumn(MessageTableCellWithChevron(I18n::Message::CopyColumnInList)),
  m_importList(MessageTableCellWithChevron(I18n::Message::ImportList)),
  m_selectableTableView(SelectableTableView(this, this, 1, Metric::CommonTopMargin, Metric::CommonRightMargin,
    Metric::CommonBottomMargin, Metric::CommonLeftMargin)),
  m_store(store),
  m_xColumnSelected(true)
{
}

void StoreParameterController::selectXColumn(bool xColumnSelected) {
  m_xColumnSelected = xColumnSelected;
}

const char * StoreParameterController::title() {
  return I18n::translate(I18n::Message::ColumnOptions);
}

View * StoreParameterController::view() {
  return &m_selectableTableView;
}

void StoreParameterController::didBecomeFirstResponder() {
  m_selectableTableView.selectCellAtLocation(0, 0);
  app()->setFirstResponder(&m_selectableTableView);
}

bool StoreParameterController::handleEvent(Ion::Events::Event event) {
  if (event == Ion::Events::OK) {
    switch (m_selectableTableView.selectedRow()) {
      case 0:
      {
        if (m_xColumnSelected) {
          m_store->deleteAllPairs();
        } else {
          m_store->resetColumn(!m_xColumnSelected);
        }
        StackViewController * stack = ((StackViewController *)parentResponder());
        stack->pop();
        return true;
      }
      case 1:
        return false;
      case 2:
        return false;
      default:
        assert(false);
        return false;
    }
  }
  return false;
}

int StoreParameterController::numberOfRows() {
  return k_totalNumberOfCell;
};

HighlightCell * StoreParameterController::reusableCell(int index) {
  assert(index >= 0);
  assert(index < k_totalNumberOfCell);
  HighlightCell * cells[] = {&m_deleteColumn, &m_copyColumn, &m_importList};
  return cells[index];
}

int StoreParameterController::reusableCellCount() {
  return k_totalNumberOfCell;
}

KDCoordinate StoreParameterController::cellHeight() {
  return Metric::ParameterCellHeight;
}

}
