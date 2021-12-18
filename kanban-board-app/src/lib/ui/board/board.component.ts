import { Component, Input } from '@angular/core';
import { ColumnModel } from '../../../lib/data-access/models/columnModel';

@Component({
  selector: 'board',
  templateUrl: './board.component.html',
  styleUrls: ['./board.component.scss'],
})
export class BoardComponent {
  @Input()
  title = 'Let me guide you!';

  @Input()
  columns: ColumnModel[] = [
    {
      id: 0,
      name: 'to do',
      position: 0,
      items: [
        {
          id: 0,
          title: 'first task',
          position: 0,
          timestamp: '2021-12-09T10:04:42.058Z',
        },
        {
          id: 1,
          title: 'second task',
          position: 1,
          timestamp: '2021-12-09T10:04:42.058Z',
        },
        {
          id: 2,
          title: 'third task',
          position: 2,
          timestamp: '2021-12-09T10:04:42.058Z',
        },
      ],
    },

    {
      id: 1,
      name: 'doing',
      position: 1,
      items: [
        {
          id: 0,
          title: 'my special first task',
          position: 0,
          timestamp: '2021-12-09T10:04:42.058Z',
        },
      ],
    },

    // {
    //   id: 2,
    //   name: 'done',
    //   position: 2,
    //   items: [
    //     {
    //       id: 0,
    //       title: 'first task',
    //       position: 0,
    //       timestamp: '2021-12-09T10:04:42.058Z',
    //     },
    //     {
    //       id: 1,
    //       title: 'second task',
    //       position: 1,
    //       timestamp: '2021-12-09T10:04:42.058Z',
    //     },
    //   ],
    // },
  ];

  addFunction() {
    this.columns.push({
      id: (Math.random() * 10000) | 0,
      name: '',
      position: 0,
      items: [],
    });
  }

  deleteFunction(id: number) {
    // var position: number = this.columns.findIndex((element)=> element.id == id)
    this.columns = this.columns.filter((element) => element.id != id);
  }
}
