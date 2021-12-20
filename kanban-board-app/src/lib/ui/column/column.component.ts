import { Component, EventEmitter, Input, Output } from '@angular/core';
import { ItemModel } from '../../../lib/data-access/models/itemModel';

@Component({
  selector: 'column',
  templateUrl: './column.component.html',
  styleUrls: ['./column.component.scss'],
})
export class ColumnComponent {
  @Input()
  title = 'column';
  // column: ColumnModel;
  @Input()
  id: number;

  @Input()
  items: ItemModel[];

  // @Output() deleteColumnEvent = new EventEmitter<number>();

  // deleteColumn(id: number) {
  //   this.deleteColumnEvent.emit(id);
  // }

  // showDeleteButton: boolean = false;

  @Output()
  changeTitleEvent = new EventEmitter<string>();
  onChangeTitle(event) {
    this.changeTitleEvent.emit(event.target.value);
  }

  deleteFunction(item: ItemModel) {
    this.items.splice(this.items.indexOf(item), 1);
  }
}
