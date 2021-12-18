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

  @Input()
  id: number;

  @Input()
  items: ItemModel[];

  @Output() deleteColumnEvent = new EventEmitter<number>();

  deleteColumn(id: number) {
    this.deleteColumnEvent.emit(id);
  }

  addEmptyItem() {
    this.items.push({
      id: 0,
      title: '',
      position: 0,
      timestamp: '',
    });
  }

  showDeleteButton: boolean = false;
}
